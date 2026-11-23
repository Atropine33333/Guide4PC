# 第2节：程序架构与代码概览

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 正演求解器（HeatSolver） | 负责按给定参数向前推进温度场的模块，即“模拟器” |
| 反演优化器（InverseOptimizer） | 负责根据观测数据反推未知参数的模块，即“学习器” |
| 网格容器（Grid2D） | 存储二维物理量（温度、扩散率、热源）的数据结构 |
| 行优先存储 | 二维数据按行连续存放在内存中，C++/C 的默认布局方式 |
| 显式欧拉 | 前向时间推进方法，新值完全由旧值计算，无需解方程 |
| 前向模拟 | 从初始温度开始，按时间步迭代得到最终温度的过程 |
| 扰动法 | 通过微调参数并观察损失变化来估算梯度的方法 |
| RMSprop | 自适应学习率优化算法，让每个参数有独立的更新步长 |
| 梯度裁剪 | 将梯度限制在 [-1, 1] 范围内，防止单次更新过大导致振荡 |
| 多时间损失 | 同时比较多个中间时刻的预测温度与观测温度，提高稳定性 |
| 物理投影 | 将更新后的参数强制限制在物理合理的上下界内 |
| OpenMP | 共享内存并行编程标准，用几行指令就能将循环分给多个CPU核心 |
| 数据竞争 | 多个线程同时读写同一块内存导致的结果不确定 |
| 负载均衡 | 让多个线程承担大致相等的工作量，避免某些核心空闲 |

---

## 2.1 总体程序结构

`thermal.cpp` 的核心设计遵循 **DeepFDM 理念**：将物理规律（热传导方程）直接编码为程序的计算图，而非用通用的神经网络去“黑箱拟合”数据。整个程序分为三个层次：

1. **数据层**（`Grid2D`）：管理所有网格数据，负责内存布局和边界处理。
2. **物理层**（`HeatSolver`）：实现正演模拟——给定参数，算温度。
3. **学习层**（`InverseOptimizer`）：实现反演训练——给定观测温度，反向推出参数。

`main` 函数担任“总指挥”的角色，依次完成：构造真实参数 → 生成观测数据 → 初始化猜测 → 循环训练 → 输出结果。

```cpp
int main() {
    // 1. 定义网格和物理常数
    const int NX = 40, NY = 20;
    const double Lx = 1.0, Ly = 0.5;
    const double dx = Lx/(NX-1), dy = Ly/(NY-1);

    // 2. 构造真实热扩散率场（左侧0.1隔热，右侧0.8导热）
    Grid2D Alpha_true(NX, NY);
    // ... 赋值 ...

    // 3. 生成高斯热源
    Grid2D Source(NX, NY);
    // ... 赋值 ...

    // 4. 用真实参数正演，记录多个中间时刻作为观测数据
    Grid2D T = T_init;
    for (int k = 1; k <= steps_target; ++k) {
        solver.stepForward(T, Alpha_true, Source, vx, vy);
        if (k % record_every == 0)
            target_records.push_back(T);
    }

    // 5. 初始猜测：全为0.5
    Grid2D Alpha_guess(NX, NY);
    std::fill(Alpha_guess.data.begin(), Alpha_guess.data.end(), 0.5);

    // 6. 创建反演优化器并开始训练
    InverseOptimizer optim(...);
    for (int iter = 1; iter <= train_iters; ++iter)
        optim.trainStep(Alpha_guess, Source, T_init, vx, vy);
}
```

**为什么这么设计：** 正演（生成数据）和反演（学习参数）共用同一个 `HeatSolver`，确保训练时的模拟逻辑与生成观测数据时的逻辑完全一致。如果两者不同，反演出来的参数可能只是在“错误的模拟器”上最优，而不是真实的物理参数。这是 DeepFDM 可解释性的根基：**程序学到的就是物理方程中的系数**，而非黑箱参数。

---

## 2.2 Grid2D 类：数据存储与内存布局

`Grid2D` 是最底层的数据容器，它把二维物理量（温度、扩散率、热源）存储为一维向量，并提供基于 `(i, j)` 坐标的存取接口。

```cpp
class Grid2D {
public:
    int nx, ny;
    std::vector<double> data;

    Grid2D(int nx_ = 0, int ny_ = 0) : nx(nx_), ny(ny_), data(nx_ * ny_, 0.0) {}

    double& at(int i, int j) { return data[j * nx + i]; }
    const double& at(int i, int j) const { return data[j * nx + i]; }

    void setBoundary(double value) {
        for (int i = 0; i < nx; ++i) {
            at(i, 0) = value;          // 下边界
            at(i, ny - 1) = value;     // 上边界
        }
        for (int j = 0; j < ny; ++j) {
            at(0, j) = value;          // 左边界
            at(nx - 1, j) = value;     // 右边界
        }
    }
};
```

**设计思路：**

**为什么用一维向量存二维数据：**
C++ 中 `std::vector<std::vector<double>>` 虽然直观，但每一行是独立分配的内存块，各行在内存中不一定连续。遍历时频繁跳转会导致 CPU 缓存失效（Cache Miss），严重影响性能。一维向量保证所有数据连续存储，访问 `data[j * nx + i]` 时，同一行的相邻列在物理地址上紧挨着。

**为什么索引公式是 `j * nx + i`（行优先）：**
C/C++ 的多维数组默认是行优先（Row-Major）布局。当内层循环遍历 `i`（列）时，地址递增步长为 1，正好是 CPU 预取数据最友好的模式。如果换成 `i * ny + j`（列优先），内层循环遍历 `j` 时地址跳跃 `ny` 步长，缓存效率大幅下降。本程序所有双重循环都是 `for (j) { for (i) { ... } }`，内层 `i` 连续，正是为行优先布局定制的。

**边界处理的必要性：**
拉普拉斯算子（见 1.5 节）计算时需要访问中心点的上下左右四个邻居。如果中心点位于边界，它的某个邻居就会落在网格之外。`setBoundary` 保证了边界值始终存在且已知，这样内点计算可以统一处理，无需在每次 `stepForward` 中做边界条件判断，既简化了代码也提升了性能。

---

## 2.3 HeatSolver 类：正向物理引擎

`HeatSolver` 封装了所有与正演相关的逻辑：离散格式、时间步长管理和损失计算。

```cpp
class HeatSolver {
public:
    double dx, dy, dt;

    HeatSolver(double dx_, double dy_, double dt_)
        : dx(dx_), dy(dy_), dt(dt_) {}

    void stepForward(Grid2D& T, const Grid2D& Alpha,
                     const Grid2D& Source, double vx, double vy) {
        Grid2D T_old = T;   // 深拷贝 — 显式欧拉的关键

        for (int j = 1; j < T.ny - 1; ++j) {
            for (int i = 1; i < T.nx - 1; ++i) {
                double lap = (T_old.at(i-1,j) + T_old.at(i+1,j) +
                              T_old.at(i,j-1) + T_old.at(i,j+1) -
                              4.0 * T_old.at(i,j)) / (dx * dx);

                double dTx = (vx >= 0.0)
                                 ? (T_old.at(i,j) - T_old.at(i-1,j)) / dx
                                 : (T_old.at(i+1,j) - T_old.at(i,j)) / dx;
                double dTy = (vy >= 0.0)
                                 ? (T_old.at(i,j) - T_old.at(i,j-1)) / dy
                                 : (T_old.at(i,j+1) - T_old.at(i,j)) / dy;

                T.at(i,j) = T_old.at(i,j) +
                            dt * (Alpha.at(i,j) * lap -
                                  vx * dTx - vy * dTy +
                                  Source.at(i,j));
            }
        }
        T.setBoundary(0.0);
    }

    static double computeMSE(const Grid2D& a, const Grid2D& b) {
        double sum = 0.0;
        for (size_t k = 0; k < a.data.size(); ++k) {
            double diff = a.data[k] - b.data[k];
            sum += diff * diff;
        }
        return sum / static_cast<double>(a.data.size());
    }
};
```

**设计思路：**

**为什么每次 `stepForward` 要拷贝 `T_old`：**
显式欧拉格式的定义是：\(T^{n+1}\) 完全由 \(T^n\) 计算。如果直接修改 `T`，当计算到点 `(i, j)` 时，它的左邻居 `(i-1, j)` 可能已经被更新为 \(T^{n+1}\)，而右邻居 `(i+1, j)` 还是 \(T^n\)。这样一来，拉普拉斯算子的计算混入了不同时间层的值，变成了“半隐式半显式”，数学上不再等价于原方程。深拷贝保证了所有计算基于同一时刻的旧值，更新全部完成后统一写入，这是显式格式正确性的底线。

**为什么边界在循环外单独设置：**
循环只遍历 `1` 到 `nx-2` 和 `1` 到 `ny-2` 的内点。边界点没有被更新，但可能因为之前的值残留非零。`setBoundary(0.0)` 每步都强制清零，确保狄利克雷边界条件在每个时间层都被精确满足。如果遗漏这一步，边界上的数值误差会向内扩散，污染整个温度场。

**`computeMSE` 为什么遍历一维向量而非二维：**
一维遍历的优点是内存连续。对于 800 个点，二维双重循环每次访问都要做 `j * nx + i` 的乘法计算；一维遍历只是 `k++`，地址自增。编译器还能将 `for` 循环自动向量化（SIMD），用一条 CPU 指令同时计算多个 `diff` 的平方。这是一种极细微但累积效果显著的优化。

---

## 2.4 InverseOptimizer 类：反向学习引擎

`InverseOptimizer` 是反演训练的“大脑”。它管理优化算法的状态（RMSprop 的累积平方梯度），并执行梯度计算与参数更新。

```cpp
class InverseOptimizer {
public:
    HeatSolver* solver_template;
    double lr, eps, alpha_max, rho, eps_rms;
    int sim_steps, record_every;
    std::vector<Grid2D>* target_records;

    Grid2D grad_sq_avg;   // RMSprop 状态

    void trainStep(Grid2D& Alpha, const Grid2D& Source,
                   const Grid2D& T_init, double vx, double vy) {
        // 1. 根据当前 Alpha 计算 CFL 安全时间步长
        double max_a = *std::max_element(Alpha.data.begin(), Alpha.data.end());
        double dt_crit = solver_template->dx * solver_template->dx / (4.0 * max_a);
        double dt_safe = 0.5 * dt_crit;
        solver_template->setDt(dt_safe);

        // 2. 初始化 RMSprop 状态
        if (grad_sq_avg.nx == 0) {
            grad_sq_avg = Grid2D(Alpha.nx, Alpha.ny);
            std::fill(grad_sq_avg.data.begin(), grad_sq_avg.data.end(), 0.0);
        }

        // 3. 用中心差分扰动计算梯度（并行）
        // ... (见 2.5 节详解) ...

        // 4. RMSprop 归一化
        for (int j = 0; j < ny; ++j)
            for (int i = 0; i < nx; ++i) {
                double g = grad.at(i,j);
                grad_sq_avg.at(i,j) = rho * grad_sq_avg.at(i,j) + (1.0 - rho) * g * g;
                grad.at(i,j) = g / (std::sqrt(grad_sq_avg.at(i,j)) + eps_rms);
            }

        // 5. 梯度裁剪
        for (int j = 0; j < ny; ++j)
            for (int i = 0; i < nx; ++i)
                grad.at(i,j) = std::clamp(grad.at(i,j), -1.0, 1.0);

        // 6. 参数更新 + 物理投影
        for (int j = 0; j < ny; ++j)
            for (int i = 0; i < nx; ++i) {
                double new_val = Alpha.at(i,j) - lr * grad.at(i,j);
                Alpha.at(i,j) = std::clamp(new_val, 0.001, alpha_max);
            }
    }
};
```

**设计思路：**

**为什么用指针 `solver_template` 而非直接持有 `HeatSolver`：**
`HeatSolver` 是有状态的（`dt` 随时间步变化），且反演中梯度计算需要多个线程各自独立运行正演（见 2.5 节）。如果 `InverseOptimizer` 直接持有一个 `HeatSolver` 对象，多线程共享时会互相干扰。用指针持有“模板”，每个线程基于它拷贝构造出自己的本地求解器，互不冲突。这也体现了“数据与操作分离”的设计原则——`InverseOptimizer` 只负责优化逻辑，不绑定特定的求解器实例。

**RMSprop 状态 `grad_sq_avg` 的懒初始化：**
`trainStep` 第一次被调用时，`grad_sq_avg.nx == 0`，此时分配内存并填零。这允许我们构造 `InverseOptimizer` 时暂不指定网格大小，等到 `trainStep` 执行时从 `Alpha` 的尺寸自动适配。避免了构造函数参数列表过长，也增强了代码的灵活性。

**为什么梯度要裁剪到 [-1, 1]：**
有限差分梯度（见 1.10 节）在远离热源的区域可能异常小（\(10^{-5}\) 量级），而在热源附近可能异常大（\(10^0\) 量级）。RMSprop 虽然能平衡量级差异，但偶尔会遇到数值噪声导致某个点的梯度异常巨大（例如 \(10^3\)），单次更新会把那个点的 `Alpha` 从 0.5 弹出到 0.001 或 1.0，破坏整个训练过程。裁剪是一个“安全网”，防止这种极端情况。经验表明，[-1, 1] 的裁剪边界对这个问题效果良好。

---

## 2.5 并行策略：OpenMP 下的梯度计算

梯度计算是程序的计算瓶颈——单次完整梯度需要 1600 次正演。利用 OpenMP 将 800 个网格点的扰动计算分配到 CPU 的所有核心上，是程序能在一两分钟内完成训练的关键。

```cpp
Grid2D Alpha_backup(nx, ny);
Alpha_backup.copyFrom(Alpha);   // 保存更新前的基准状态

#pragma omp parallel
{
    // 每个线程拥有独立的求解器、预测场和扰动场
    HeatSolver local_solver(solver_template->dx,
                            solver_template->dy,
                            dt_safe);
    Grid2D T_pred(nx, ny);
    Grid2D Alpha_pert(nx, ny);
    Alpha_pert.copyFrom(Alpha_backup);

    #pragma omp for collapse(2) schedule(dynamic)
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            double orig = Alpha_backup.at(i, j);

            // +ε 扰动
            Alpha_pert.at(i, j) = orig + eps;
            double Lplus = runSimulation(T_pred, Alpha_pert,
                                         Source, vx, vy,
                                         local_solver, T_init);

            // -ε 扰动
            Alpha_pert.at(i, j) = orig - eps;
            double Lminus = runSimulation(T_pred, Alpha_pert,
                                          Source, vx, vy,
                                          local_solver, T_init);

            Alpha_pert.at(i, j) = orig;
            grad.at(i, j) = (Lplus - Lminus) / (2.0 * eps);
        }
    }
}
```

**设计思路：**

**为什么每个线程要独立创建 `HeatSolver` 和 `Grid2D`：**
如果多个线程共享同一个 `T_pred` 或同一个 `local_solver`，一个线程正在执行 `stepForward` 修改 `T_pred` 时，另一个线程可能正在读取它来计算损失，这就是典型的**数据竞争**。即便通过互斥锁保护，也会让并行执行退化为串行（线程频繁等待锁）。每个线程拥有自己的副本后，所有读写操作只发生在线程本地内存，无需任何同步开销，真正实现“各算各的，最后汇总”。

**`collapse(2)` 的作用：**
默认情况下，`#pragma omp for` 只并行最外层的 `j` 循环。20 个 `j` 分配到 8 个线程上，平均每个线程 2.5 个 `j`，意味着内层的 40 个 `i` 是串行执行的。`collapse(2)` 将双重循环“展平”为 800 次独立迭代，然后均匀分配到各个线程。这样可以更充分地利用 CPU 核心，避免某些线程因分到的 `j` 层工作量大而成为瓶颈。

**为什么用 `schedule(dynamic)` 而非默认的 `static`：**
`static` 调度在编译期就把迭代平均分给线程。但不同网格点的计算量并不完全相等——靠近中心的点，温度场变化剧烈，`runSimulation` 的计算量相同，但系统调度可能让某个线程在运行时被中断。`dynamic` 调度允许“先完成的多做”，实现运行时负载均衡。虽然动态调度有少量额外开销，但对于每次迭代都要跑 8000 步正演的重计算任务，这点开销完全可以忽略，而负载均衡带来的收益是显著的。

**`Alpha_backup` 存在的必要性：**
在并行循环中，`Alpha` 是当前训练步的初始值。如果不备份，当第一个线程扰动点 `(0,0)` 时，修改了 `Alpha.at(0,0)`；紧接着第二个线程在计算点 `(0,1)` 的梯度时，`runSimulation` 使用的 `Alpha` 中 `(0,0)` 的值已经被扰动过，不再是基准状态。这会导致梯度计算公式中“只改变目标点，其余点不变”的前提被破坏，算出的梯度是错误的方向。`Alpha_backup` 保存了干净的基准状态，每次扰动基于它恢复，确保每个点的梯度计算独立且正确。

---

## 2.6 多时间损失与训练监控

本程序采用“多时间损失”——不只比较最终时刻的温度，而是比较正演过程中 16 个等间隔记录点的温度。

```cpp
double runSimulation(Grid2D& T_pred, const Grid2D& Alpha,
                     const Grid2D& Source, double vx, double vy,
                     HeatSolver& solver, const Grid2D& T_init) {
    T_pred.copyFrom(T_init);
    T_pred.setBoundary(0.0);
    double loss_sum = 0.0;
    int rec_idx = 0;

    for (int k = 1; k <= sim_steps; ++k) {
        solver.stepForward(T_pred, Alpha, Source, vx, vy);
        if (k % record_every == 0 && rec_idx < (int)target_records->size()) {
            loss_sum += HeatSolver::computeMSE(T_pred, (*target_records)[rec_idx]);
            ++rec_idx;
        }
    }
    return loss_sum / rec_idx;
}
```

**设计思路：**

**为什么比较多个时刻而不是只看终点：**
热传导方程是扩散型的，早期时刻的温度场主要反映局部热源附近的扩散，晚期时刻则反映整个区域的平均特性。如果只比较终点，反演问题可能“病态”——多种不同的 `Alpha` 分布可能在终点温度上差异不大，但在中间时刻差异显著。多时间损失相当于给优化器提供了“时间维度的监督信号”，强制预测的整个演化轨迹都匹配观测数据，大大降低了反演的不唯一性。这也是为什么程序生成观测数据时记录了 16 个时刻，而非只保存最后一步。

**训练日志中的监控信息：**

```cpp
// 每次梯度计算后输出统计量
double gmax = *std::max_element(grad.data.begin(), grad.data.end());
double gmin = *std::min_element(grad.data.begin(), grad.data.end());
double gavg = 0.0;
for (double g : grad.data) gavg += std::abs(g);
gavg /= grad.data.size();
double g_mon = grad.at(8, 9);   // 低导热区内的一个点

std::ostringstream oss;
oss << "    [Grad] min=" << gmin << " max=" << gmax
    << " avg|grad|=" << gavg << " g(8,9)=" << g_mon;
log_write(oss.str());
```

**为什么输出 `grad.at(8, 9)`：**
点 `(8, 9)` 位于左半区域（低导热通道内，约 `x = 8/40 = 0.2`，`y = 9/20 = 0.45`）。监控这个特定点的梯度，可以直观地看到反演过程中低导热区的参数如何变化。如果训练正常，这个点的梯度初期较大（因为它离热源较近），后期逐渐减小到零（参数接近真值）。如果梯度一直很大，说明学习率可能偏小；如果梯度反复振荡，说明学习率可能偏大。这个监控值帮助我们在训练中快速诊断优化状态。

**损失报告中的 `Left(8,9)`：**
每次迭代报告 `α ∈ [amin, amax]` 和 `Left(8,9)` 的当前值。从 `inversion.log` 可以看到，`Left(8,9)` 从初始的 0.5 逐渐下降到约 0.108，最终接近真值 0.1。这个动态反映了反演过程的收敛趋势，也是 DeepFDM 可解释性的直接体现——我们追踪的每个数字都是有明确物理意义的扩散系数，而不是抽象的网络权重。

---

## 2.7 本节代码结构总览

| 模块 | 文件位置 | 核心职责 | 关键设计决策 |
| :--- | :--- | :--- | :--- |
| `Grid2D` | 第 30-65 行 | 数据存储与边界管理 | 行优先一维向量，`setBoundary` 强制清零 |
| `HeatSolver` | 第 70-130 行 | 正演模拟与损失计算 | 显式欧拉，迎风差分，深拷贝 `T_old` |
| `runSimulation` | 第 170-190 行 | 单次正演 + 多时间损失汇总 | 返回多个时刻的平均 MSE |
| `InverseOptimizer` | 第 140-260 行 | 梯度计算与参数更新 | 中心差分扰动，RMSprop，梯度裁剪 |
| `main` | 第 300-450 行 | 流程编排与 IO | 生成真值 → 训练 → 输出 CSV |

---

## 后续学习建议

1. 先通读 `main` 函数，把握程序“生成数据 → 初始化 → 训练 → 输出”的完整流程。
2. 进入 `InverseOptimizer::trainStep`，理解梯度计算的两层循环是如何利用 OpenMP 拆分的。
3. 观察 `inversion.log` 中损失从 `14.95` 降到 `0.092` 的过程，对比 `Left(8,9)` 从 0.5 收敛到 0.108，直观感受梯度下降的效果。
4. 对比 `alpha_true.csv` 和 `alpha_inferred.csv`，左半区域的数值从约 0.1 被恢复到了约 0.1，右半区域保持约 0.8——这正是 DeepFDM 要证明的核心：物理编码的模型能精准反演出有明确物理含义的参数。