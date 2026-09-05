# Section 2: Program Architecture and Code Overview

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Forward solver (HeatSolver) | The module responsible for advancing the temperature field forward in time given the parameters – the "simulator" |
| Inverse optimizer (InverseOptimizer) | The module responsible for inferring unknown parameters from observed data – the "learner" |
| Grid container (Grid2D) | The data structure that stores 2D physical quantities (temperature, diffusivity, heat source) |
| Row‑major storage | 2D data stored contiguously in memory row by row – C/C++'s default layout |
| Explicit Euler | A forward time‑stepping method where new values are computed entirely from old values without solving equations |
| Forward simulation | The process of iterating from an initial temperature to obtain the final temperature field |
| Perturbation method | A technique that estimates gradients by slightly tweaking parameters and observing how the loss changes |
| RMSprop | An adaptive learning‑rate optimisation algorithm that gives each parameter its own update step size |
| Gradient clipping | Restricting gradients to the [-1, 1] range to prevent excessively large single updates that cause oscillations |
| Multi‑time loss | Compares predicted and observed temperatures at multiple intermediate time instants simultaneously, improving stability |
| Physical projection | Forcing updated parameters back into physically reasonable lower and upper bounds |
| OpenMP | A shared‑memory parallel programming standard – with a few directives, it distributes loops across multiple CPU cores |
| Data race | An indeterminate result caused by multiple threads reading and writing the same memory location concurrently |
| Load balancing | Distributing roughly equal workloads across threads so no core sits idle while others are overloaded |

---

## 2.1 Overall Program Structure

The core design of `thermal.cpp` follows the **DeepFDM philosophy**: encode the laws of physics (the heat equation) directly into the program's computational graph, rather than using a generic neural network to "black‑box fit" the data. The entire program is organised into three layers:

1. **Data layer** (`Grid2D`): manages all grid data, handling memory layout and boundary conditions.
2. **Physics layer** (`HeatSolver`): implements forward simulation – given parameters, compute the temperature.
3. **Learning layer** (`InverseOptimizer`): implements inverse training – given observed temperatures, infer the parameters backward.

The `main` function acts as the overall conductor, sequentially: constructing the true parameters → generating observed data → initialising a guess → training in a loop → outputting results.

```cpp
int main() {
    // 1. Define grid and physical constants
    const int NX = 40, NY = 20;
    const double Lx = 1.0, Ly = 0.5;
    const double dx = Lx/(NX-1), dy = Ly/(NY-1);

    // 2. Construct the true thermal diffusivity field (left side 0.1 insulating, right side 0.8 conducting)
    Grid2D Alpha_true(NX, NY);
    // ... assign values ...

    // 3. Generate the Gaussian heat source
    Grid2D Source(NX, NY);
    // ... assign values ...

    // 4. Forward‑simulate with the true parameters, recording multiple intermediate instants as observations
    Grid2D T = T_init;
    for (int k = 1; k <= steps_target; ++k) {
        solver.stepForward(T, Alpha_true, Source, vx, vy);
        if (k % record_every == 0)
            target_records.push_back(T);
    }

    // 5. Initial guess: all set to 0.5
    Grid2D Alpha_guess(NX, NY);
    std::fill(Alpha_guess.data.begin(), Alpha_guess.data.end(), 0.5);

    // 6. Create the inverse optimizer and start training
    InverseOptimizer optim(...);
    for (int iter = 1; iter <= train_iters; ++iter)
        optim.trainStep(Alpha_guess, Source, T_init, vx, vy);
}
```

**Why this design:** both the forward pass (generating data) and the inverse pass (learning parameters) share the same `HeatSolver`. This ensures that the simulation logic used during training is exactly the same as the one used to generate the observations. If they differed, the inferred parameters might only be optimal for a "wrong simulator" rather than representing true physical parameters. This is the foundation of DeepFDM's interpretability: **the program learns the coefficients of the physical equation**, not black‑box weights.

---

## 2.2 The Grid2D Class – Data Storage and Memory Layout

`Grid2D` is the lowest‑level data container. It stores 2D physical quantities (temperature, diffusivity, source) in a 1D vector and provides access via `(i, j)` coordinates.

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
            at(i, 0) = value;          // bottom boundary
            at(i, ny - 1) = value;     // top boundary
        }
        for (int j = 0; j < ny; ++j) {
            at(0, j) = value;          // left boundary
            at(nx - 1, j) = value;     // right boundary
        }
    }
};
```

**Design rationale:**

**Why store 2D data in a 1D vector:**
In C++, `std::vector<std::vector<double>>` is intuitive, but each row is a separately allocated memory block – rows are not necessarily contiguous in memory. Traversing such a structure causes frequent cache misses, significantly hurting performance. A 1D vector guarantees that all data is stored contiguously. When accessing `data[j * nx + i]`, adjacent columns in the same row are right next to each other in physical memory.

**Why the index formula is `j * nx + i` (row‑major):**
C/C++ multidimensional arrays use row‑major layout by default. When the inner loop traverses `i` (columns), the address stride is exactly 1, which is the most cache‑friendly access pattern for CPU prefetching. If we used `i * ny + j` (column‑major), traversing `j` in the inner loop would jump by `ny` each time, drastically reducing cache efficiency. All double loops in this program are `for (j) { for (i) { ... } }`, with `i` as the inner loop – this is deliberately tailored to the row‑major layout.

**Why boundary handling is necessary:**
The Laplacian (see Section 1.5) requires access to the four neighbours of the centre point. If the centre point lies on the boundary, one of its neighbours would fall outside the grid. `setBoundary` ensures that boundary values always exist and are known, so the interior computation can be handled uniformly without having to check boundary conditions inside every `stepForward` call. This both simplifies the code and improves performance.

---

## 2.3 The HeatSolver Class – The Forward Physics Engine

`HeatSolver` encapsulates all forward‑simulation logic: discretisation schemes, time‑step management, and loss computation.

```cpp
class HeatSolver {
public:
    double dx, dy, dt;

    HeatSolver(double dx_, double dy_, double dt_)
        : dx(dx_), dy(dy_), dt(dt_) {}

    void stepForward(Grid2D& T, const Grid2D& Alpha,
                     const Grid2D& Source, double vx, double vy) {
        Grid2D T_old = T;   // deep copy – essential for explicit Euler

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

**Design rationale:**

**Why copy `T_old` on every `stepForward`:**
The explicit Euler scheme is defined as: \(T^{n+1}\) is computed entirely from \(T^n\). If we modified `T` in place, by the time we compute point `(i, j)`, its left neighbour `(i-1, j)` might already have been updated to \(T^{n+1}\), while its right neighbour `(i+1, j)` is still \(T^n\). The Laplacian would then mix values from different time levels, becoming "semi‑implicit semi‑explicit" – mathematically no longer equivalent to the original equation. The deep copy ensures that all computations are based on the same old time level, and writes are only committed after the entire update is complete. This is the baseline correctness requirement for explicit schemes.

**Why boundaries are set outside the loop:**
The loop only traverses interior points from `1` to `nx-2` and `1` to `ny-2`. Boundary points are not updated, but they might retain non‑zero values from previous steps. `setBoundary(0.0)` forces them to zero at every time step, ensuring that the Dirichlet boundary condition is exactly satisfied at every time level. Omitting this would allow boundary errors to diffuse inward and contaminate the entire temperature field.

**Why `computeMSE` iterates over the 1D vector rather than 2D:**
1D traversal has the advantage of contiguous memory. For 800 points, a 2D double loop would perform a `j * nx + i` multiplication for each access; 1D traversal is just `k++` with auto‑incrementing addresses. The compiler can also auto‑vectorise the loop (SIMD), computing the squares of multiple `diff` values with a single CPU instruction. This is a subtle but cumulatively significant optimisation.

---

## 2.4 The InverseOptimizer Class – The Inverse Learning Engine

`InverseOptimizer` is the "brain" of the inverse training. It manages the state of the optimisation algorithm (RMSprop's accumulated squared gradients) and performs both gradient computation and parameter updates.

```cpp
class InverseOptimizer {
public:
    HeatSolver* solver_template;
    double lr, eps, alpha_max, rho, eps_rms;
    int sim_steps, record_every;
    std::vector<Grid2D>* target_records;

    Grid2D grad_sq_avg;   // RMSprop state

    void trainStep(Grid2D& Alpha, const Grid2D& Source,
                   const Grid2D& T_init, double vx, double vy) {
        // 1. Compute the CFL‑safe time step from the current Alpha
        double max_a = *std::max_element(Alpha.data.begin(), Alpha.data.end());
        double dt_crit = solver_template->dx * solver_template->dx / (4.0 * max_a);
        double dt_safe = 0.5 * dt_crit;
        solver_template->setDt(dt_safe);

        // 2. Initialise RMSprop state (lazy)
        if (grad_sq_avg.nx == 0) {
            grad_sq_avg = Grid2D(Alpha.nx, Alpha.ny);
            std::fill(grad_sq_avg.data.begin(), grad_sq_avg.data.end(), 0.0);
        }

        // 3. Compute the gradient via central‑difference perturbation (parallelised)
        // ... (see Section 2.5 for details) ...

        // 4. RMSprop normalisation
        for (int j = 0; j < ny; ++j)
            for (int i = 0; i < nx; ++i) {
                double g = grad.at(i,j);
                grad_sq_avg.at(i,j) = rho * grad_sq_avg.at(i,j) + (1.0 - rho) * g * g;
                grad.at(i,j) = g / (std::sqrt(grad_sq_avg.at(i,j)) + eps_rms);
            }

        // 5. Gradient clipping
        for (int j = 0; j < ny; ++j)
            for (int i = 0; i < nx; ++i)
                grad.at(i,j) = std::clamp(grad.at(i,j), -1.0, 1.0);

        // 6. Parameter update + physical projection
        for (int j = 0; j < ny; ++j)
            for (int i = 0; i < nx; ++i) {
                double new_val = Alpha.at(i,j) - lr * grad.at(i,j);
                Alpha.at(i,j) = std::clamp(new_val, 0.001, alpha_max);
            }
    }
};
```

**Design rationale:**

**Why use a pointer `solver_template` rather than holding a `HeatSolver` directly:**
`HeatSolver` is stateful (its `dt` changes with the time step), and gradient computation during inversion requires multiple threads to run forward simulations independently (see Section 2.5). If `InverseOptimizer` held a single `HeatSolver` instance, multiple threads sharing it would interfere with each other. Using a pointer to a "template" allows each thread to construct its own local solver by copying from it – no conflicts. This also reflects the design principle of "separation of data and operations": `InverseOptimizer` only handles optimisation logic; it does not bind to a particular solver instance.

**Lazy initialisation of the RMSprop state `grad_sq_avg`:**
The first time `trainStep` is called, `grad_sq_avg.nx == 0` – at that point memory is allocated and zero‑filled. This allows us to construct `InverseOptimizer` without knowing the grid size upfront; it automatically adapts to `Alpha`'s dimensions when `trainStep` runs. This keeps the constructor parameter list short and makes the code more flexible.

**Why clip gradients to [-1, 1]:**
The finite‑difference gradient (see Section 1.10) can be abnormally small (around \(10^{-5}\)) in regions far from the heat source, and abnormally large (around \(10^0\)) near the source. While RMSprop balances these magnitude differences, occasional numerical noise can produce an outlier gradient at a single point – say \(10^3\) – which would drive that point's `Alpha` from 0.5 all the way to 0.001 or 1.0 in one step, destabilising the entire training process. Clipping acts as a safety net. Experience shows that [-1, 1] works well for this problem.

---

## 2.5 Parallelisation Strategy – Computing Gradients with OpenMP

Gradient computation is the computational bottleneck – a single full gradient requires 1600 forward simulations. Using OpenMP to distribute the 800 grid‑point perturbation calculations across all available CPU cores is what enables the program to complete training within a minute or two.

```cpp
Grid2D Alpha_backup(nx, ny);
Alpha_backup.copyFrom(Alpha);   // save the baseline state before perturbing

#pragma omp parallel
{
    // Each thread gets its own independent solver, prediction field, and perturbation field
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

            // +ε perturbation
            Alpha_pert.at(i, j) = orig + eps;
            double Lplus = runSimulation(T_pred, Alpha_pert,
                                         Source, vx, vy,
                                         local_solver, T_init);

            // -ε perturbation
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

**Design rationale:**

**Why create independent `HeatSolver` and `Grid2D` instances per thread:**
If multiple threads shared the same `T_pred` or `local_solver`, one thread might be modifying `T_pred` inside `stepForward` while another thread reads it for loss computation – a classic **data race**. Even if we protected it with mutual‑exclusion locks, parallel execution would degrade to serial (threads constantly waiting for the lock). By giving each thread its own copies, all reads and writes occur in thread‑local memory, requiring no synchronisation overhead – true "each computes independently, then aggregate".

**What `collapse(2)` does:**
By default, `#pragma omp for` only parallelises the outermost `j` loop. With 20 `j` values distributed across 8 threads, each thread gets roughly 2.5 `j`s – meaning the inner 40 `i`s are processed serially. `collapse(2)` flattens the double loop into 800 independent iterations and distributes them evenly across threads. This makes better use of CPU cores and prevents some threads from becoming bottlenecks due to uneven workload distribution across `j`.

**Why use `schedule(dynamic)` rather than the default `static`:**
`static` scheduling divides iterations evenly at compile time. But the computational cost of each grid point isn't exactly the same – though the `runSimulation` workload is identical, system scheduling may interrupt a thread at runtime. `dynamic` scheduling allows threads to pick up new iterations as they finish ("the ones that finish early do more"), achieving runtime load balancing. While dynamic scheduling has a small overhead, it is negligible compared to the heavy computation (8000 time steps per simulation), and the benefit of balanced load is significant.

**Why `Alpha_backup` is necessary:**
Inside the parallel loop, `Alpha` is the initial state for the current training step. Without a backup, when the first thread perturbs point `(0,0)`, it modifies `Alpha.at(0,0)`. Immediately after, when the second thread computes the gradient at point `(0,1)`, the `Alpha` used in `runSimulation` already has `(0,0)` perturbed – no longer in its baseline state. This violates the assumption that "only the target point changes while all others remain fixed", producing an incorrect gradient direction. `Alpha_backup` preserves a clean baseline, and every perturbation is restored from it, ensuring that each point's gradient is independent and correct.

---

## 2.6 Multi‑Time Loss and Training Monitoring

This program uses a "multi‑time loss" – instead of comparing only the final temperature, it compares the temperatures at 16 equally spaced recording instants during the forward simulation.

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

**Design rationale:**

**Why compare at multiple instants rather than just the final time:**
The heat equation is diffusive – early‑time temperature fields mainly reflect local diffusion around the heat source, while late‑time fields reflect the overall average characteristics of the entire domain. If we compared only the final state, the inverse problem could be **ill‑posed**: many different `Alpha` distributions might produce similar final temperatures, even if they differ significantly at intermediate times. The multi‑time loss provides the optimiser with "supervisory signals along the time dimension", forcing the predicted evolution trajectory to match the observations at every stage. This greatly reduces the non‑uniqueness of the inversion. That is why the program recorded 16 instants when generating the observations, rather than just the final step.

**Monitoring information in the training logs:**

```cpp
// After each gradient computation, output statistics
double gmax = *std::max_element(grad.data.begin(), grad.data.end());
double gmin = *std::min_element(grad.data.begin(), grad.data.end());
double gavg = 0.0;
for (double g : grad.data) gavg += std::abs(g);
gavg /= grad.data.size();
double g_mon = grad.at(8, 9);   // a point inside the low‑conductivity region

std::ostringstream oss;
oss << "    [Grad] min=" << gmin << " max=" << gmax
    << " avg|grad|=" << gavg << " g(8,9)=" << g_mon;
log_write(oss.str());
```

**Why output `grad.at(8, 9)`:**
Point `(8, 9)` lies in the left‑half region (inside the low‑conductivity channel, roughly at `x = 8/40 = 0.2`, `y = 9/20 = 0.45`). Monitoring the gradient at this specific point gives a direct visual indication of how the low‑conductivity region evolves during inversion. In a normal training run, this gradient is initially large (because the point is relatively close to the heat source) and gradually shrinks to zero (as the parameter approaches its true value). If the gradient remains large, the learning rate may be too small; if it oscillates, the learning rate may be too large. This monitor helps us diagnose optimisation health quickly during training.

**What `Left(8,9)` means in the loss report:**
Every iteration reports the current value of `α ∈ [amin, amax]` and `Left(8,9)`. From `inversion.log`, you can see `Left(8,9)` starting at 0.5, gradually dropping to about 0.108, eventually approaching the true value of 0.1. This dynamic directly reflects the convergence trend of the inversion, and is a direct demonstration of DeepFDM's interpretability – every number we track has a clear physical meaning (a diffusion coefficient), not an abstract network weight.

---

## 2.7 Section Code Structure Overview

| Module | Line Range | Core Responsibility | Key Design Decisions |
| :--- | :--- | :--- | :--- |
| `Grid2D` | lines 30–65 | Data storage and boundary management | Row‑major 1D vector; `setBoundary` forces zero on edges |
| `HeatSolver` | lines 70–130 | Forward simulation and loss computation | Explicit Euler; upwind differencing; deep copy of `T_old` |
| `runSimulation` | lines 170–190 | Single forward pass + multi‑time loss aggregation | Returns average MSE across multiple recorded instants |
| `InverseOptimizer` | lines 140–260 | Gradient computation and parameter update | Central‑difference perturbation; RMSprop; gradient clipping |
| `main` | lines 300–450 | Orchestration and I/O | Generate ground truth → train → output CSV |

---

## Suggestions for Further Study

1. Read through `main` first to grasp the overall flow: generate data → initialise → train → output.
2. Dive into `InverseOptimizer::trainStep` and understand how the two‑loop gradient computation is split using OpenMP.
3. Watch the loss drop from `14.95` to `0.092` in `inversion.log`, and track `Left(8,9)` as it converges from 0.5 to 0.108 – this gives an intuitive feel for how gradient descent works.
4. Compare `alpha_true.csv` and `alpha_inferred.csv`: the left region has been recovered from roughly 0.1 to roughly 0.1, and the right region stays at about 0.8 – exactly the core result DeepFDM aims to demonstrate: a physics‑encoded model can accurately recover parameters that have clear physical meaning.