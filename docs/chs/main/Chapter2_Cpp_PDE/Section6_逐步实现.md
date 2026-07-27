# 第6节：最小可行产品到完整工程

> 推荐先看一看杂谈，这里面有我的心路历程。

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 最小可行产品（MVP） | 只保留核心功能、去掉所有花哨特性的最简可运行版本 |
| 增量开发 | 每次只加一个小功能，确保它正常后再加下一个，避免bug堆积 |
| 真值验证 | 先构造一个已知答案的问题，看算法能否找回答案，这是验证反演算法的标准方法 |
| 可识别性 | 观测数据能否唯一确定待求参数的性质，可识别性差则反演会失败 |
| 关注点分离 | 数据存储、物理计算、优化迭代放在不同的类里，互不干扰 |
| 线程局部数据 | 每个线程拥有自己的数据副本，避免并行时的数据竞争 |
| 梯度裁剪 | 限制单次更新的最大幅度，防止数值异常导致训练崩溃 |
| 物理投影 | 强制参数在物理合理的上下界内，保证CFL条件始终可满足 |
| 局部等效模型 | 反演只能恢复"对观测有影响"的参数区域，不敏感区域保持不变 |
| 多时间损失 | 用多个时刻的温度作监督信号，增强参数可识别性 |

---

## 6.1 总体原则：从"能跑"到"好用"

任何科学计算程序的开发都应该遵循"先跑通、再调优"的原则。这个项目也是如此。整个开发过程可以清晰地分为三个阶段：

1. **最小可行产品（MVP）**：让程序能跑起来，能算梯度、能更新参数、能看到损失在下降。这个阶段不考虑性能、不关心收敛速度、不在乎代码美观，唯一的目标是**证明思路正确**。

2. **功能完善**：加入多时间损失、RMSprop、梯度裁剪、物理投影，让收敛更稳定、结果更合理。

3. **性能优化**：加入OpenMP并行，缩短训练时间。

下面我会按照这个顺序，一步步还原代码的生长过程。

---

## 6.2 MVP 第一步：数据容器（Grid2D）

**核心诉求**：我需要一个东西来存二维网格数据。它要能分配内存、能读写任意网格点，最好还能自动处理边界。

```cpp
// 第一版：只存数据、只提供访问
class Grid2D {
public:
    int nx, ny;
    std::vector<double> data;

    Grid2D(int nx_, int ny_) : nx(nx_), ny(ny_), data(nx_ * ny_, 0.0) {}

    double& at(int i, int j) {
        return data[j * nx + i];
    }
};
```

三样东西就够用了：构造函数、成员变量、存取函数。这时候还没写`setBoundary`，还没写拷贝构造函数，因为暂时用不到。等边界操作反复出现时再把它提到类里：

```cpp
// 第二版：加入边界操作
void setBoundary(double value) {
    for (int i = 0; i < nx; ++i) {
        at(i, 0) = value;
        at(i, ny - 1) = value;
    }
    for (int j = 0; j < ny; ++j) {
        at(0, j) = value;
        at(nx - 1, j) = value;
    }
}
```

**设计思路**：
- 一维向量+行优先存储，既让内存连续又让缓存友好。
- 只有真正需要时才加功能，不提前设计"完美"的类。
- 边界是物理问题的一部分，数据容器负责存储边界值和清零操作。

---

## 6.3 MVP 第二步：前向求解器（HeatSolver）

**核心诉求**：我需要一个函数，输入当前温度场和物理参数，输出下一时刻的温度场。

```cpp
// 第一版：只算扩散，忽略对流
class HeatSolver {
public:
    double dx, dy, dt;

    HeatSolver(double dx_, double dy_, double dt_) 
        : dx(dx_), dy(dy_), dt(dt_) {}

    void stepForward(Grid2D& T, const Grid2D& Alpha, const Grid2D& Source) {
        Grid2D T_old = T;   // 深拷贝，显式欧拉的关键

        for (int j = 1; j < T.ny - 1; ++j) {
            for (int i = 1; i < T.nx - 1; ++i) {
                double lap = (T_old.at(i-1, j) + T_old.at(i+1, j) +
                              T_old.at(i, j-1) + T_old.at(i, j+1) -
                              4.0 * T_old.at(i, j)) / (dx * dx);

                T.at(i, j) = T_old.at(i, j) + 
                             dt * (Alpha.at(i, j) * lap + Source.at(i, j));
            }
        }
        T.setBoundary(0.0);
    }
};
```

**为什么MVP阶段不写对流**：对流项( `-vx*dTx - vy*dTy`)需要处理迎风差分，虽然代码不长，但会引入条件分支。MVP的目标是"先跑通"，所以先排除所有不必要的复杂性。等扩散项验证正确了，再追加对流项。

**验证方法**：用均匀Alpha=0.5、中心高斯热源跑正演，观察温度场是否呈圆形对称扩散。如果是，说明拉普拉斯算子和显式欧拉的实现正确。

```cpp
// 第二版：加入对流项
void stepForward(Grid2D& T, const Grid2D& Alpha, const Grid2D& Source,
                 double vx, double vy) {
    Grid2D T_old = T;

    for (int j = 1; j < T.ny - 1; ++j) {
        for (int i = 1; i < T.nx - 1; ++i) {
            double lap = ...;

            // 迎风差分：根据速度方向选择前向或后向差分
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
```

验证方法：设置 `vx=0.3, vy=0.0`，观察温度场是否整体向右漂移。如果是，迎风差分正确。

---

## 6.4 MVP 第三步：生成观测数据

**核心诉求**：我需要一份"真实观测数据"来验证反演算法。最直接的办法：构造一个已知的Alpha场，用正演跑出温度场，把这个当"观测"。

```cpp
// 构造真值Alpha场
Grid2D Alpha_true(NX, NY);
for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        double x = i * dx;
        Alpha_true.at(i, j) = (x < 0.5) ? 0.1 : 0.8;
    }
}

// 构造高斯热源
Grid2D Source(NX, NY);
const double src_amp = 500.0, src_sigma = 0.05, src_x0 = 0.3, src_y0 = 0.25;
for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        double x = i * dx, y = j * dy;
        double r2 = (x - src_x0)*(x - src_x0) + (y - src_y0)*(y - src_y0);
        Source.at(i, j) = src_amp * exp(-r2 / (2.0 * src_sigma * src_sigma));
    }
}

// 正演生成观测数据
Grid2D T = T_init;
for (int k = 0; k < steps_target; ++k) {
    solver.stepForward(T, Alpha_true, Source, 0.0, 0.0);
}
```

**为什么真值是"左低右高"**：答案越直观越好。左半边0.1（隔热）、右半边0.8（导热），中间有明显台阶。反演如果能把台阶找回来，程序就正确。

**为什么热源放在低导区**：热源在低导区内部时，低导率对温度场的"困住热量"效应最强，观测信号最敏感。如果热源远离低导区，热量根本扩散不到那里，反演对那个区域的Alpha就毫无敏感性——这是反演中"可识别性"的基本问题。

---

## 6.5 MVP 第四步：梯度计算

**核心诉求**：我需要计算损失函数对每个Alpha网格点的偏导数。损失通过8000步正演算出，没有解析表达式，所以用数值扰动。

```cpp
// MVP版本：串行，只算单时刻损失
Grid2D grad(NX, NY);
Grid2D Alpha_backup = Alpha;

for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        double orig = Alpha_backup.at(i, j);

        // +ε 扰动
        Alpha.at(i, j) = orig + eps;
        Grid2D T_pred = T_init;
        for (int k = 0; k < steps_target; ++k)
            solver.stepForward(T_pred, Alpha, Source, 0.0, 0.0);
        double Lplus = computeMSE(T_pred, T_target);

        // -ε 扰动
        Alpha.at(i, j) = orig - eps;
        T_pred = T_init;
        for (int k = 0; k < steps_target; ++k)
            solver.stepForward(T_pred, Alpha, Source, 0.0, 0.0);
        double Lminus = computeMSE(T_pred, T_target);

        Alpha.at(i, j) = orig;
        grad.at(i, j) = (Lplus - Lminus) / (2.0 * eps);
    }
}
```

这个版本非常慢——800个点×2次扰动×8000步 = 1280万次网格更新——但**它能算对**。MVP阶段的唯一目标是验证"梯度方向正确、损失能下降"。

**验证方法**：跑一次完整的梯度计算，更新一次Alpha，看损失是否减少。如果损失减少了，说明梯度方向对了。

**关于摄动步长eps的选择**：`eps=0.01`。太小了损失差被浮点舍入误差淹没，太大了超出线性近似范围。这是一个经验值，在本问题中表现稳定。

**关于为什么不用自动微分**：自动微分需要依赖第三方框架（如PyTorch）我就是为了教Cpp语法的，自然不用。

---

## 6.6 MVP 第五步：优化迭代

**核心诉求**：有了梯度，我就可以更新Alpha，让损失逐步下降。

```cpp
// MVP版本：纯梯度下降，无任何自适应
double lr = 1.0;
for (int iter = 0; iter < 200; ++iter) {
    // 计算grad...
    for (int j = 0; j < NY; ++j) {
        for (int i = 0; i < NX; ++i) {
            double new_val = Alpha.at(i, j) - lr * grad.at(i, j);
            Alpha.at(i, j) = std::clamp(new_val, 0.001, 1.0);
        }
    }
    // 报告当前损失...
}
```

这就是最原始的反演循环。它只有四样东西：
1. 计算梯度（见6.5节）
2. 沿梯度反方向更新（`Alpha -= lr * grad`）
3. 物理投影（`clamp`到`[0.001, 1.0]`）
4. 报告损失监控进度

这个版本已经能收敛了。在热源位于低导区的配置下，50次迭代左右就能看到Alpha左侧明显下降、右侧明显上升。

---

## 6.7 功能完善：从MVP到可用版本

MVP跑通后，我开始加功能，让收敛更稳定、结果更合理。

### 6.7.1 加入多时间损失

只比较最终时刻时，解空间太大——很多不同的Alpha分布在终点温度上差异不大。加上中间时刻后，优化器必须同时匹配温度演化的全过程。

```cpp
// 生成观测时多存几个时刻
std::vector<Grid2D> target_records;
for (int k = 1; k <= steps_target; ++k) {
    solver.stepForward(T, Alpha_true, Source, 0, 0);
    if (k % record_every == 0)
        target_records.push_back(T);
}

// 计算损失时求所有时刻的平均
double computeLoss(Grid2D& T_pred, const std::vector<Grid2D>& target_records) {
    double sum = 0.0;
    for (size_t m = 0; m < target_records.size(); ++m) {
        sum += computeMSE(T_pred, target_records[m]);
    }
    return sum / target_records.size();
}
```

这一步让反演稳定性大幅提升，因为梯度信号从"时间终点"扩散到了"整个时间轴"。

### 6.7.2 加入RMSprop

纯梯度下降的问题是：热源附近的Alpha梯度大、更新快；远离热源的梯度小、更新慢。RMSprop通过维护每个参数的历史梯度平方，自适应调整步长。

```cpp
Grid2D grad_sq_avg(NX, NY);
// 在每次迭代中：
for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        double g = grad.at(i, j);
        grad_sq_avg.at(i, j) = rho * grad_sq_avg.at(i, j) + (1.0 - rho) * g * g;
        grad.at(i, j) = g / (sqrt(grad_sq_avg.at(i, j)) + eps_rms);
    }
}
```

这保证了热源中心和远处的更新步长在同一个量级，避免了中心震荡、远处不动的尴尬局面。

### 6.7.3 加入梯度裁剪

虽然RMSprop能平衡量级，但偶尔会有某个点的梯度因为数值原因异常大（例如1e3），一次更新就能把该点的Alpha从0.5弹到0.001或1.0。梯度裁剪是防止这种极端情况的安全网。

```cpp
for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        grad.at(i, j) = std::clamp(grad.at(i, j), -1.0, 1.0);
    }
}
```

### 6.7.4 调整学习率

MVP阶段用的是`lr=1.0`，收敛快但在接近最优时震荡。最终版本调到`lr=0.002`，收敛更平滑。

---

## 6.8 性能优化：OpenMP并行

MVP版本的串行梯度计算，200次迭代要好几个小时小时，不利于调试。

**观察**：800个网格点的梯度计算完全独立——点`(i,j)`的梯度不依赖点`(i',j')`的结果。这是典型的"尴尬并行"。

```cpp
#pragma omp parallel
{
    // 每个线程拥有独立的数据副本
    HeatSolver local_solver(solver_template->dx, solver_template->dy, dt_safe);
    Grid2D T_pred(NX, NY);
    Grid2D Alpha_pert(NX, NY);
    Alpha_pert.copyFrom(Alpha_backup);

    #pragma omp for collapse(2) schedule(dynamic)
    for (int j = 0; j < NY; ++j) {
        for (int i = 0; i < NX; ++i) {
            // 用local_solver和T_pred计算Lplus、Lminus
        }
    }
}
```

**关键设计**：
- 每个线程自己创建`HeatSolver`和`T_pred`，互不干扰。
- `schedule(dynamic)`让先完成的线程多承担任务，负载更均衡。
- `Alpha_backup`保存基准状态，保证每个点的扰动都基于同一参考值。

加速比约13-15倍（16核CPU），训练时间从好几个小时缩短到15分钟以内。

---

## 6.9 结果解读：什么是"合理的反演结果"

最终反演的Alpha场不会完美复现"左0.1右0.8"的真值。实际的典型结果是：
- 热源附近（x≈0.3, y≈0.25）：Alpha下降到0.10-0.15
- 右侧大部分区域：上升到0.75-0.85
- 左侧远离热源的区域：保持在0.4-0.5（初始猜测）
- 边界附近：几乎不变

**这是正常的，不是失败**。反演的本质是"只恢复对观测数据有影响的参数"。热源附近的变化对温度场影响大，所以被恢复；远处的影响小，梯度≈0，所以保持初始猜测。

这恰好引出了反演中一个更深层的问题：**如何设计观测方案（热源位置、边界条件、记录时长）使待求参数都可识别？** 这比"调参让收敛更快"更有讨论价值。

---

## 6.10 开发流程总结

如果要从零开始复现这个项目，推荐的步骤是：

| 顺序 | 功能模块 | 验证方法 | 状态 |
|:---|:---|:---|:---|
| 1 | `Grid2D`（存取+边界） | 填值打印，确认布局 | MVP |
| 2 | `HeatSolver`（扩散项） | 中心热源对称扩散 | MVP |
| 3 | `HeatSolver`（对流项） | 温度场整体平移 | 功能扩展 |
| 4 | 生成真值+观测数据 | 保存CSV，可视化检查 | MVP |
| 5 | 梯度计算（串行） | 跑一次，确认损失下降 | MVP |
| 6 | 优化循环（纯SGD） | 观察Alpha场变化 | MVP |
| 7 | 多时间损失 | 比较单时刻vs多时刻收敛 | 功能完善 |
| 8 | RMSprop | 观察收敛曲线更平滑 | 功能完善 |
| 9 | 梯度裁剪 | 防止偶尔的数值爆炸 | 功能完善 |
| 10 | OpenMP并行 | 对比单核/多核运行时间 | 性能优化 |

每一步都保持"可编译、可运行、可验证"的状态，确保问题被及时定位，而不是在写了500行代码后才发现不收敛。