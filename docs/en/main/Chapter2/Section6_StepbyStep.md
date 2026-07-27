# Section 6: From Minimum Viable Product to Complete Project

> I recommend reading Tittle-tattle first – it contains my personal reflections on the journey.

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Minimum Viable Product (MVP) | The simplest runnable version that keeps only core functionality and strips away all bells and whistles |
| Incremental development | Add one small feature at a time, make sure it works, then add the next – prevents bugs from piling up |
| Ground‑truth validation | Construct a problem with a known answer and see whether the algorithm can recover it – the standard way to validate inversion algorithms |
| Identifiability | Whether the observed data can uniquely determine the parameter you're looking for – poor identifiability causes inversion to fail |
| Separation of concerns | Data storage, physics computation, and optimisation are placed in different classes that don't interfere with each other |
| Thread‑local data | Each thread gets its own copy of data to avoid data races during parallel execution |
| Gradient clipping | Limits the maximum single‑step update magnitude to prevent numerical anomalies from crashing the training |
| Physical projection | Forces parameters to stay within physically reasonable bounds, ensuring the CFL condition can always be satisfied |
| Local effective model | Inversion can only recover parameter regions that "affect the observations" – insensitive regions stay unchanged |
| Multi‑time loss | Uses temperatures at multiple time instants as supervisory signals, enhancing parameter identifiability |

---

## 6.1 Overall Principle: From "It Runs" to "It Works Well"

Every scientific computing project should follow the principle of "get it working first, then make it better". This project is no exception. The entire development process can be clearly divided into three phases:

1. **Minimum Viable Product (MVP)**: Get the program to run – compute gradients, update parameters, and watch the loss decrease. This phase ignores performance, convergence speed, and code beauty. The only goal is to **prove the idea works**.

2. **Feature completion**: Add multi‑time loss, RMSprop, gradient clipping, and physical projection to make convergence more stable and results more reasonable.

3. **Performance optimisation**: Add OpenMP parallelism to shorten training time.

Below, I'll walk through the code's growth process step by step, following this exact order.

---

## 6.2 MVP Step 1: The Data Container (Grid2D)

**Core requirement**: I need something to store 2D grid data. It must allocate memory, allow reading/writing at any grid point, and ideally handle boundaries automatically.

```cpp
// First version: just stores data, just provides access
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

Three things are enough: constructor, member variables, access function. At this stage, I didn't write `setBoundary`, didn't write a copy constructor – because I didn't need them yet. Only when boundary operations started appearing repeatedly did I move them into the class:

```cpp
// Second version: added boundary operations
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

**Design rationale**:
- A 1D vector plus row‑major storage provides both memory contiguity and cache friendliness.
- Add functionality only when actually needed – don't design a "perfect" class upfront.
- Boundaries are part of the physics problem; the data container is responsible for storing boundary values and zeroing operations.

---

## 6.3 MVP Step 2: The Forward Solver (HeatSolver)

**Core requirement**: I need a function that takes the current temperature field and physical parameters, and produces the temperature field at the next time step.

```cpp
// First version: diffusion only, no advection
class HeatSolver {
public:
    double dx, dy, dt;

    HeatSolver(double dx_, double dy_, double dt_) 
        : dx(dx_), dy(dy_), dt(dt_) {}

    void stepForward(Grid2D& T, const Grid2D& Alpha, const Grid2D& Source) {
        Grid2D T_old = T;   // deep copy – essential for explicit Euler

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

**Why not implement advection in the MVP phase**: The advection term (`-vx*dTx - vy*dTy`) requires handling upwind differencing, which introduces conditional branches. Although the code is short, it adds complexity. The MVP goal is "get it running", so I stripped away all non‑essential complexity. Only after verifying the diffusion term was correct did I add advection.

**Verification method**: Run a forward simulation with uniform Alpha=0.5 and a central Gaussian heat source, and observe whether the temperature field spreads symmetrically outward. If yes, the Laplacian and explicit Euler implementations are correct.

```cpp
// Second version: added advection
void stepForward(Grid2D& T, const Grid2D& Alpha, const Grid2D& Source,
                 double vx, double vy) {
    Grid2D T_old = T;

    for (int j = 1; j < T.ny - 1; ++j) {
        for (int i = 1; i < T.nx - 1; ++i) {
            double lap = ...;

            // Upwind differencing: choose forward/backward based on velocity direction
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

**Verification method**: Set `vx=0.3, vy=0.0` and observe whether the temperature field drifts to the right as a whole. If yes, the upwind differencing is correct.

---

## 6.4 MVP Step 3: Generating Observations

**Core requirement**: I need a set of "real observational data" to validate the inversion algorithm. The most straightforward way: construct a known Alpha field, run the forward simulation to produce a temperature field, and treat that as the "observation".

```cpp
// Construct the ground‑truth Alpha field
Grid2D Alpha_true(NX, NY);
for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        double x = i * dx;
        Alpha_true.at(i, j) = (x < 0.5) ? 0.1 : 0.8;
    }
}

// Construct the Gaussian heat source
Grid2D Source(NX, NY);
const double src_amp = 500.0, src_sigma = 0.05, src_x0 = 0.3, src_y0 = 0.25;
for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        double x = i * dx, y = j * dy;
        double r2 = (x - src_x0)*(x - src_x0) + (y - src_y0)*(y - src_y0);
        Source.at(i, j) = src_amp * exp(-r2 / (2.0 * src_sigma * src_sigma));
    }
}

// Forward simulation to generate observations
Grid2D T = T_init;
for (int k = 0; k < steps_target; ++k) {
    solver.stepForward(T, Alpha_true, Source, 0.0, 0.0);
}
```

**Why "left low, right high"**: The simpler the answer, the better. Left half 0.1 (insulating), right half 0.8 (conducting), with a clear step in between. If the inversion can recover that step, the program works.

**Why place the heat source inside the low‑conductivity region**: When the source is inside the low‑conductivity region, the effect of "trapping heat" is strongest, and the observational signal is most sensitive. If the source were far away, heat would never reach that region, and the inversion would have zero sensitivity there – the fundamental identifiability issue in inverse problems.

---

## 6.5 MVP Step 4: Gradient Computation

**Core requirement**: I need to compute the partial derivative of the loss function with respect to each Alpha grid point. The loss is computed through an 8000‑step forward simulation, so there is no analytic expression – numerical perturbation is required.

```cpp
// MVP version: serial, single‑time loss only
Grid2D grad(NX, NY);
Grid2D Alpha_backup = Alpha;

for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        double orig = Alpha_backup.at(i, j);

        // +ε perturbation
        Alpha.at(i, j) = orig + eps;
        Grid2D T_pred = T_init;
        for (int k = 0; k < steps_target; ++k)
            solver.stepForward(T_pred, Alpha, Source, 0.0, 0.0);
        double Lplus = computeMSE(T_pred, T_target);

        // -ε perturbation
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

This version is very slow – 800 points × 2 perturbations × 8000 steps = 12.8 million grid updates – but **it computes correctly**. The sole goal of the MVP phase is to verify that "the gradient direction is correct and the loss decreases".

**Verification method**: Run one full gradient computation, update Alpha once, and see if the loss decreases. If it does, the gradient direction is correct.

**Choosing the perturbation step size eps**: `eps=0.01`. Any smaller and the loss difference is drowned by floating‑point round‑off error; any larger and it exceeds the linear approximation regime. This is an empirical value that proves stable for this problem.

**Why not use automatic differentiation**: AD requires external frameworks (like PyTorch). Since the point of this project is to teach C++ syntax, I deliberately avoided them.

---

## 6.6 MVP Step 5: Optimisation Iteration

**Core requirement**: With the gradient, I can update Alpha and gradually reduce the loss.

```cpp
// MVP version: plain gradient descent, no adaptation
double lr = 1.0;
for (int iter = 0; iter < 200; ++iter) {
    // compute grad...
    for (int j = 0; j < NY; ++j) {
        for (int i = 0; i < NX; ++i) {
            double new_val = Alpha.at(i, j) - lr * grad.at(i, j);
            Alpha.at(i, j) = std::clamp(new_val, 0.001, 1.0);
        }
    }
    // report current loss...
}
```

This is the raw inversion loop. It does only four things:
1. Compute the gradient (see Section 6.5)
2. Update in the opposite direction of the gradient (`Alpha -= lr * grad`)
3. Physical projection (`clamp` to `[0.001, 1.0]`)
4. Report loss to monitor progress

This version already converges. With the heat source placed inside the low‑conductivity region, after about 50 iterations you can see the left side of Alpha drop noticeably and the right side rise noticeably.

---

## 6.7 Feature Completion: From MVP to a Usable Version

After the MVP ran, I started adding features to make convergence more stable and results more reasonable.

### 6.7.1 Adding Multi‑Time Loss

When comparing only the final time, the solution space is too large – many different Alpha distributions produce similar final temperatures. Adding intermediate instants forces the optimiser to match the entire temperature evolution trajectory.

```cpp
// Store multiple instants when generating observations
std::vector<Grid2D> target_records;
for (int k = 1; k <= steps_target; ++k) {
    solver.stepForward(T, Alpha_true, Source, 0, 0);
    if (k % record_every == 0)
        target_records.push_back(T);
}

// Average the loss across all recorded instants
double computeLoss(Grid2D& T_pred, const std::vector<Grid2D>& target_records) {
    double sum = 0.0;
    for (size_t m = 0; m < target_records.size(); ++m) {
        sum += computeMSE(T_pred, target_records[m]);
    }
    return sum / target_records.size();
}
```

This step dramatically improved inversion stability, because the gradient signal spread from "the final time" to "the entire time axis".

### 6.7.2 Adding RMSprop

The problem with plain gradient descent is that Alpha near the heat source has large gradients and updates quickly, while Alpha far from the source has small gradients and updates slowly. RMSprop maintains a moving average of the squared gradients for each parameter, adaptively adjusting the step size.

```cpp
Grid2D grad_sq_avg(NX, NY);
// Inside each iteration:
for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        double g = grad.at(i, j);
        grad_sq_avg.at(i, j) = rho * grad_sq_avg.at(i, j) + (1.0 - rho) * g * g;
        grad.at(i, j) = g / (sqrt(grad_sq_avg.at(i, j)) + eps_rms);
    }
}
```

This ensures that the update step sizes at the heat‑source centre and in the far corners are of the same order, preventing centre oscillation and corner stagnation.

### 6.7.3 Adding Gradient Clipping

Although RMSprop balances magnitudes, occasionally a single point's gradient may become abnormally large due to numerical issues (e.g., 1e3), causing that point's Alpha to jump from 0.5 to 0.001 or 1.0 in one step. Gradient clipping acts as a safety net.

```cpp
for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        grad.at(i, j) = std::clamp(grad.at(i, j), -1.0, 1.0);
    }
}
```

### 6.7.4 Tuning the Learning Rate

The MVP phase used `lr=1.0`, which converged quickly but oscillated near the optimum. The final version was tuned to `lr=0.002`, yielding smoother convergence.

---

## 6.8 Performance Optimisation: OpenMP Parallelism

The MVP's serial gradient computation made 200 iterations take hours, which was impractical for debugging.

**Observation**: Gradient computation for the 800 grid points is completely independent – the gradient at point `(i,j)` does not depend on the result at point `(i',j')`. This is a classic "embarrassingly parallel" problem.

```cpp
#pragma omp parallel
{
    // Each thread has its own independent data copies
    HeatSolver local_solver(solver_template->dx, solver_template->dy, dt_safe);
    Grid2D T_pred(NX, NY);
    Grid2D Alpha_pert(NX, NY);
    Alpha_pert.copyFrom(Alpha_backup);

    #pragma omp for collapse(2) schedule(dynamic)
    for (int j = 0; j < NY; ++j) {
        for (int i = 0; i < NX; ++i) {
            // compute Lplus and Lminus using local_solver and T_pred
        }
    }
}
```

**Key design points**:
- Each thread creates its own `HeatSolver` and `T_pred`, with no interference.
- `schedule(dynamic)` lets threads that finish early take on more tasks, balancing the load.
- `Alpha_backup` preserves the baseline state, ensuring every perturbation starts from the same reference.

Speedup is about 13‑15× (on a 16‑core CPU), cutting training time from several hours to under 15 minutes.

---

## 6.9 Interpreting Results: What Counts as a "Reasonable" Inversion Result

The final inferred Alpha field will not perfectly reproduce the "0.1 on the left, 0.8 on the right" ground truth. A typical actual result is:
- Near the heat source (x≈0.3, y≈0.25): Alpha drops to 0.10–0.15
- Most of the right region: rises to 0.75–0.85
- Left region far from the source: stays around 0.4–0.5 (the initial guess)
- Near the boundaries: barely changes

**This is normal, not a failure**. The essence of inversion is "only recover parameters that affect the observations". Changes near the heat source have a large impact on the temperature field, so they are recovered. Changes far away have little impact, so their gradient is near zero, and they keep their initial guess.

This raises a deeper question in inversion: **how should the observation scheme (heat‑source placement, boundary conditions, recording duration) be designed to make all target parameters identifiable?** That is a more meaningful discussion than "tweak parameters to converge faster".

---

## 6.10 Development Process Summary

If you were to reproduce this project from scratch, here is the recommended step‑by‑step sequence:

| Order | Module | Validation Method | Phase |
|:---|:---|:---|:---|
| 1 | `Grid2D` (access + boundaries) | Fill values, print, verify layout | MVP |
| 2 | `HeatSolver` (diffusion only) | Central source spreads symmetrically | MVP |
| 3 | `HeatSolver` (advection) | Temperature field shifts as a whole | Feature extension |
| 4 | Generate ground truth + observations | Save CSV, visual inspection | MVP |
| 5 | Gradient computation (serial) | Run once, confirm loss decreases | MVP |
| 6 | Optimisation loop (plain SGD) | Observe Alpha field changes | MVP |
| 7 | Multi‑time loss | Compare single‑time vs. multi‑time convergence | Feature completion |
| 8 | RMSprop | Observe smoother convergence curves | Feature completion |
| 9 | Gradient clipping | Prevent occasional numerical blow‑ups | Feature completion |
| 10 | OpenMP parallelism | Compare single‑core vs. multi‑core runtime | Performance optimisation |

Every step stays in a "compilable, runnable, verifiable" state, ensuring that issues are caught early rather than discovering non‑convergence after writing 500 lines of code.