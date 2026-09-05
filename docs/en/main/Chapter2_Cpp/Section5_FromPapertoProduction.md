# Section 5: From Paper to Production – How DeepFDM Was Brought to Life

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Discretisation | Turning continuous mathematical equations (PDEs) into algebraic equations that a computer can solve |
| Five‑point stencil | A discrete Laplacian that involves the centre point and its four orthogonal neighbours (up, down, left, right) |
| Upwind differencing | Choosing forward or backward differencing based on the flow direction to maintain numerical stability |
| Explicit scheme | New time‑level values are computed directly from old values – no system of equations to solve |
| CFL condition | The time step must not exceed a critical value, otherwise the numerical solution explodes |
| Dirichlet boundary | Fixed temperature on the boundary (zero in this program) – the simplest and most common boundary condition |
| Forward simulation | Computing the temperature evolution from known parameters |
| Inverse problem | Inferring unknown parameter distributions from observed temperature data |
| Loss function | A metric that measures the mismatch between predictions and observations – the "judge" of the inversion |
| Finite‑difference perturbation | Using two forward simulations (perturbing the parameter by +ε and −ε) to approximate the gradient |
| RMSprop | An adaptive learning‑rate algorithm that gives each parameter its own step size |
| Gradient clipping | Limiting single‑step updates to [-1, 1] to prevent training from going out of control |
| Multi‑time loss | Comparing temperatures at multiple intermediate instants simultaneously, adding more supervisory signals |
| Physical projection | Forcing parameters to stay within physically reasonable bounds (α cannot be negative or too large) |
| Parallelisation | Using multiple CPU cores simultaneously – distributing 800 forward simulations across 8 cores |
| Thread‑local data | Each thread has its own copy of data to avoid cross‑thread interference |
| Cache locality | When data is stored contiguously in memory, the CPU can access it much faster |
| Hyperparameters | Parameters that must be set before running the algorithm – e.g., learning rate, perturbation step size |
| Convergence | The property that the loss function gradually decreases and stabilises over iterations |
| Ill‑posed problem | A problem where the solution is not unique or is highly sensitive to noise – a common challenge in inversion |

---

## 5.1 From Mathematical Formulas to Executable Code – The Abstraction Process

The partial differential equations in a paper are written in mathematical language, which computers do not understand. The programmer's first job is to translate those few lines of formulas into data structures and algorithmic steps.

**The core equation from the paper (heat equation):**

\[
\frac{\partial T}{\partial t} = \alpha(x,y)\left(\frac{\partial^2T}{\partial x^2} + \frac{\partial^2T}{\partial y^2}\right) + S(x,y)
\]

This equation describes how temperature evolves over time. To make a computer execute it, we must perform the following abstractions:

**Step 1: Discretisation – from continuous to discrete**

| Continuous concept | Discrete implementation |
| :--- | :--- |
| Continuous space \((x,y)\) | Grid points \((i,j)\), \(x_i=i\cdot\Delta x,\ y_j=j\cdot\Delta y\) |
| Continuous time \(t\) | Time step \(n\), \(t_n=n\cdot\Delta t\) |
| Temperature field \(T(x,y,t)\) | Array \(T_{i,j}^{(n)}\), stored in a `Grid2D` |
| Thermal diffusivity \(\alpha(x,y)\) | Array \(\alpha_{i,j}\), stored in another `Grid2D` |
| Heat source \(S(x,y)\) | Array \(S_{i,j}\), pre‑computed and stored |
| Laplacian \(\Delta T\) | Five‑point finite‑difference stencil (see Section 5.3) |
| Time derivative \(\partial T/\partial t\) | Forward Euler difference \(T^{n+1} = T^n + \Delta t \cdot (\cdots)\) |

**Step 2: Forward simulation – forward iteration**

```cpp
// For each time step, iterate over all interior points, computing new values from old ones
for (int k = 0; k < sim_steps; ++k) {
    for (int j = 1; j < ny-1; ++j)
        for (int i = 1; i < nx-1; ++i)
            T_new.at(i,j) = T_old.at(i,j) + dt * (Alpha.at(i,j) * lap + Source.at(i,j));
}
```

This process is the **forward simulation** – given the parameters, compute the temperature.

**Step 3: Inversion – backward optimisation**

The "inverse problem" mentioned in the paper is transformed into an **optimisation problem** in engineering:

\[
\alpha^* = \arg\min_\alpha \sum_t \|T_\text{pred}(\alpha, t) - T_\text{obs}(t)\|^2
\]

Translated into code:

```cpp
// 1. Guess an initial α (usually uniform)
// 2. Compute loss: run a forward simulation, compare predicted temperatures with observations
// 3. Compute the gradient: perturb each grid point's α and see how the loss changes
// 4. Update: adjust α in the opposite direction of the gradient
// 5. Repeat steps 2-4 until the loss is small enough
```

This "three‑step sequence" is the core logic of the entire project: **discretisation → forward simulation → inversion**. The paper provides the mathematical description; the project provides the executable code.

---

## 5.2 Constructing the Coefficient Field – Why the Ground Truth Was Designed This Way

The way to validate an inversion algorithm is to first construct a known ground truth, simulate synthetic observations from it, and then see whether the algorithm can recover that ground truth from the data. If it cannot recover a known answer, it will certainly fail on real data.

**The ground‑truth design in this project:**

```cpp
// Left half (x < 0.5): low diffusivity 0.1 (simulating an insulating layer / air gap)
// Right half (x ≥ 0.5): high diffusivity 0.8 (simulating copper)

for (int j = 0; j < NY; ++j) {
    for (int i = 0; i < NX; ++i) {
        Alpha_true.at(i,j) = (i * dx < 0.5) ? 0.1 : 0.8;
    }
}
```

**Why this particular "left‑right" distribution:**

1. **A sharp interface exists**: at \(x=0.5\), there is a clear discontinuity – left side conducts slowly, right side conducts quickly. This type of discontinuity is the hardest feature for inversion to recover: neural networks tend to produce smooth solutions, whereas DeepFDM stores α directly on the grid, so it can naturally capture jumps.

2. **Physically interpretable**: the low‑diffusivity region can be thought of as insulation, an air gap, or a damaged area, while the high‑diffusivity region represents a metal matrix. This is a realistic scenario in engineering non‑destructive testing – finding internal defects (voids, cracks, inclusions) inside materials.

3. **The initial guess \(\alpha=0.5\) is not arbitrary**: it lies halfway between 0.1 and 0.8, giving the algorithm freedom to "decide whether to go left or right". If the initial guess were 0.1, the left half might converge quickly but the right half would have to climb up; if 0.8, the opposite. 0.5 minimises the initial bias.

---

## 5.3 The Explicit Euler Scheme – Why Choose the Simplest Option

The paper mentions the Forward Euler method, and the project directly adopts it. The trade‑offs are as follows:

**Explicit Euler is extremely simple to implement:**

```cpp
T.at(i,j) = T_old.at(i,j) + dt * (Alpha.at(i,j) * lap + Source.at(i,j));
```

One line of code is the entire update formula for one time step – no linear system to solve, the code directly mirrors the math, and it is almost impossible to get wrong.

**Why not choose a more sophisticated time‑stepping scheme (like Crank‑Nicolson or Runge‑Kutta):**

| Scheme | Advantages | Disadvantages | Decision for this project |
| :--- | :--- | :--- | :--- |
| Explicit Euler | Very simple, O(N) per step | Time step limited by CFL condition | ✅ Selected – code clarity wins |
| Crank‑Nicolson | Unconditionally stable, larger steps | Requires solving linear systems, code 10× more complex | ❌ Not selected |
| RK4 | High accuracy (O(Δt⁴)) | Four right‑hand‑side evaluations per step, gradient cost quadruples | ❌ Not selected |
| Adaptive stepping | Automatically adjusts for efficiency | Non‑fixed steps complicate gradient comparisons | ❌ Not selected |

**The inversion context amplifies the advantage of explicit schemes**: gradient computation requires 1600 forward simulations. If each forward simulation involved solving a linear system (implicit) or four extra RHS evaluations (RK4), the total cost would multiply from 1600×8000 steps to 1600×8000×(tens of times) – turning "2 minutes" into "2 hours". Although explicit Euler uses smaller time steps, each step is extremely lightweight. For an inversion scenario that runs "ten thousand forward simulations", the explicit scheme is globally optimal.

---

## 5.4 Discretising the Laplacian and Implementing Boundary Conditions in Code

**The five‑point stencil in code:**

```cpp
double lap = (T_old.at(i-1, j) + T_old.at(i+1, j) +
              T_old.at(i, j-1) + T_old.at(i, j+1) -
              4.0 * T_old.at(i, j)) / (dx * dx);
```

This stencil only uses the four orthogonal neighbours – not the diagonal ones like `(i-1,j-1)`. Why?

- **Second‑order accuracy is sufficient**: the truncation error is \(O(\Delta x^2)\). If we refine the grid from 40×20 to 80×40, the error drops to one‑quarter – more than enough to resolve the left‑right contrast in α.
- **Low computational cost**: the five‑point stencil requires only 5 memory accesses. A nine‑point stencil (including diagonals) would need 9 accesses – almost double the work, with only marginal accuracy gains.
- **Isotropy**: on a square grid, this stencil is perfectly symmetric in the x and y directions, so it introduces no directional bias.

**Engineering boundary‑condition handling:**

```cpp
// The loop only processes interior points (i=1..nx-2, j=1..ny-2)
for (int j = 1; j < T.ny - 1; ++j) {
    for (int i = 1; i < T.nx - 1; ++i) {
        // ... update interior points ...
    }
}
// After each step, force the boundary to zero
T.setBoundary(0.0);
```

**Why handle boundaries separately instead of adding `if` checks inside the loop:**

If we added `if (i==0 || i==nx-1 || j==0 || j==ny-1)` inside the interior loop, we would perform a conditional check every iteration. Over 800 points × 8000 steps × 1600 forward simulations ≈ 10.2 billion checks – a huge cumulative overhead. The boundary consists of only 2×40+2×20-4=116 points (14.5% of the grid). Handling those 116 points in a separate loop leaves the main loop (684 points) branch‑free, maximising CPU pipeline efficiency.

---

## 5.5 Turning the Inverse Problem into an Optimisation Problem

The paper says "the inverse problem is ill‑posed". Engineering needs to turn it into a "computable" problem. This project uses **mean squared error + gradient descent** to solve it.

**Loss function design:**

```cpp
// Single‑time MSE
double mse = (1.0 / N) * Σ (T_pred[k] - T_target[k])²
```

This looks simple, but **why use squared error instead of absolute error**?

- Squared error penalises large deviations more heavily (a difference of 2 gives a penalty of 4, while 0.5 gives 0.25), pushing the optimiser to eliminate large errors first.
- The derivative of squared error is continuous: \(\partial(\text{MSE})/\partial T = 2(T_{pred} - T_{target})/N\). Absolute error is non‑differentiable at zero, which causes numerical trouble for gradient computation.
- Squared error corresponds statistically to a **Gaussian noise assumption** – if the observation errors are normally distributed, MSE is the maximum‑likelihood estimator.

**The actual loss in the code is "multi‑time averaged":**

```cpp
// Record every 500 steps, for a total of 16 recording points
for (int k = 1; k <= sim_steps; ++k) {
    solver.stepForward(T_pred, Alpha, Source, vx, vy);
    if (k % record_every == 0) {
        loss_sum += computeMSE(T_pred, target_records[rec_idx++]);
    }
}
return loss_sum / rec_idx;  // average over the 16 time instants
```

If we compared only the final time, the "solution space" of the inverse problem would be huge – many different α distributions might produce similar final temperatures because the diffusion process has already smoothed out early details. Adding intermediate instants forces the optimiser to match temperatures at all times simultaneously, yielding a stronger constraint and a more unique solution.

---

## 5.6 Why 8000 Forward Simulation Steps

From the code, `steps_target = 8000` and `record_every = 500`, giving 16 recorded instants.

**How 8000 steps were determined:**

```cpp
// Physical time = time step × number of steps
dt_true = 0.5 * (dx * dx) / (4.0 * max_alpha)
        = 0.5 * (0.0256²) / (4.0 * 0.8)
        ≈ 0.0001027 seconds per step

Total physical time = 8000 × 0.0001027 ≈ 0.822 seconds
```

Why run up to 0.8 seconds?

- The characteristic time for heat to diffuse from the source (at x=0.3) across the whole domain (Lx=1.0) is roughly \(L_x^2 / \alpha \approx 1.0^2 / 0.8 = 1.25\) seconds. 0.8 seconds covers the process from the source spreading out to most of the domain.
- If we used too few steps (e.g., 2000, physical time 0.2 s), the heat would not have reached the right boundary yet, and the observations would contain only information near the source – making it difficult to infer α on the right side.
- If we used too many steps (e.g., 20000), the system would be near steady state (temperature no longer changing), and extra time steps would add computational cost without providing new information.

```cpp
// During inversion, the time step is recomputed from the current guess of Alpha
double max_a = *std::max_element(Alpha.data.begin(), Alpha.data.end());
double dt_crit = solver_template->dx * solver_template->dx / (4.0 * max_a);
double dt_safe = 0.5 * dt_crit;
```

**Important**: the time step during inversion may differ from the one used to generate the ground truth! If the current guess of α_max is larger than the true value (say 0.6), then `dt_safe` becomes smaller than the `dt_true` used in the forward truth simulation. That means the same 8000 steps now cover a shorter physical time. The predicted temperature evolution will lag behind the observations, increasing the loss. This very deviation provides the gradient direction – the optimiser will tend to reduce α_max to make the time step longer, bringing the evolution speed back in line with the observations.

---

## 5.7 Engineering Implementation of Gradient Computation

The paper does not specify how to compute the gradient – that is an engineering choice. This project uses **central finite‑difference perturbation**.

```cpp
#pragma omp parallel
{
    // Each thread has its own solver copy
    HeatSolver local_solver(solver_template->dx, solver_template->dy, dt_safe);
    Grid2D T_pred(nx, ny);
    Grid2D Alpha_pert(nx, ny);
    Alpha_pert.copyFrom(Alpha_backup);

    #pragma omp for collapse(2) schedule(dynamic)
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            double orig = Alpha_backup.at(i, j);

            // +ε perturbation
            Alpha_pert.at(i, j) = orig + eps;
            double Lplus = runSimulation(T_pred, Alpha_pert, ...);

            // -ε perturbation
            Alpha_pert.at(i, j) = orig - eps;
            double Lminus = runSimulation(T_pred, Alpha_pert, ...);

            Alpha_pert.at(i, j) = orig;
            grad.at(i, j) = (Lplus - Lminus) / (2.0 * eps);
        }
    }
}
```

**Engineering trade‑off: why not use Automatic Differentiation (AD)?**

If we used a framework like TensorFlow/PyTorch, AD would be built‑in. But this project is pure C++ (no third‑party dependencies). Implementing AD manually would require a computational graph, back‑propagation infrastructure, etc. – ballooning the code from ~500 lines to ~5000 lines. The finite‑difference perturbation method, though computationally heavier (1600× more forward runs), is extremely simple to implement, logically clear, and has no dependencies. Given that the grid has only 800 points, 1600 forward simulations run in parallel on an 8‑core CPU take about 1‑2 minutes – entirely acceptable.

**Why the perturbation step size is 0.01:**

| eps value | Issue |
| :--- | :--- |
| Too small (e.g., 1e-8) | \(L_{+\epsilon} - L_{-\epsilon}\) approaches the floating‑point round‑off level, so the gradient is drowned by numerical noise |
| Too large (e.g., 0.1) | The perturbation goes beyond the local linear regime, and the central‑difference truncation error becomes excessive |
| 0.01 (this project) | Balances truncation and round‑off errors, and has shown stable convergence in validation |

---

## 5.8 RMSprop + Gradient Clipping – Why This Combination

**Why not plain SGD:**

```cpp
// Plain SGD update
Alpha -= lr * grad;
```

With 800 parameters, the gradient magnitudes vary enormously. The α at the heat‑source centre is highly sensitive to loss (large gradient), while α far from the source is almost unaffected (tiny gradient). Plain SGD with a single learning rate causes the centre to oscillate and the corners to barely move.

**RMSprop solves this:**

```cpp
// Each parameter maintains its own moving average of squared gradients
grad_sq_avg.at(i,j) = rho * grad_sq_avg.at(i,j) + (1.0 - rho) * g * g;
grad.at(i,j) = g / (std::sqrt(grad_sq_avg.at(i,j)) + eps_rms);
```

If a parameter's gradients are consistently large, `grad_sq_avg` is also large, so the normalised effective step size shrinks automatically. Conversely, if gradients are consistently small, the effective step size grows. This makes the optimiser "agnostic" to the parameter's identity – whether you are at the heat‑source centre or a far‑away corner, your update magnitude stays in the same ballpark.

**The role of gradient clipping:**

Even with RMSprop, occasional single‑point gradient anomalies can occur (e.g., numerical noise causing a gradient 100× larger than normal). Without handling this, that point's α jumps from 0.5 to near 0.001 or 1.0, ruining the entire training run. Clipping to [-1,1] acts as a "safety fuse".

---

## 5.9 Parallelisation Strategy – Why OpenMP Was Chosen

There are 800 grid points, each requiring 2 forward simulations, each with 8000 steps. Total workload = 800 × 2 × 8000 = 12.8 million grid updates. Running this serially on a 3 GHz CPU would take several minutes. OpenMP can distribute this work across 8 cores, reducing the wall‑clock time to around 30 seconds.

**Why OpenMP instead of more complex alternatives:**

| Solution | Advantages | Disadvantages | Suitability |
| :--- | :--- | :--- | :--- |
| OpenMP | Minimal code changes (a few #pragma), cross‑platform, low shared‑memory communication cost | Only for single‑node multi‑core, no cluster support | ✅ Selected |
| MPI | Can scale across multiple machines, theoretically unlimited | Major code changes, high communication overhead | ❌ Overkill for 800 points |
| CUDA | Extremely fast on GPUs (thousands of cores) | Requires an NVIDIA GPU, completely different programming model | ❌ Avoided to keep no GPU dependency |

**The cost‑benefit of OpenMP:**

```cpp
// Add 3 lines of code, get ~6‑7× speedup
#pragma omp parallel
{
    // ... 
    #pragma omp for collapse(2) schedule(dynamic)
    // ...
}
```

- Cost: requires compiler support (`-fopenmp`), and adds "non‑standard" `#pragma` directives.
- Benefit: training time drops from ~7 minutes to ~1 minute.
- For this 800‑point × 2‑perturbation × 8000‑step problem, this is an extremely attractive "bang for the buck".

---

## 5.10 Complete Mapping from Paper to Code

| Paper / Mathematical Concept | Engineering Implementation | Code Location |
| :--- | :--- | :--- |
| PDE definition \(T_t = \alpha\Delta T + S\) | Explicit Euler update formula | `HeatSolver::stepForward` |
| Spatial discretisation | Five‑point Laplacian stencil | `lap` variable computation |
| Temporal discretisation | Forward Euler \(T^{n+1}=T^n+\Delta t\cdot F\) | `dt * (...)` in the update formula |
| CFL stability condition | `dt_crit = dx²/(4*max_alpha)`, using 0.5× safety factor | `InverseOptimizer::trainStep` |
| Dirichlet boundary | `setBoundary(0.0)` after each step | End of `stepForward` |
| Forward simulation (given parameters, compute temperature) | `runSimulation` loop calling `stepForward` | `runSimulation` function |
| Inverse problem (infer parameters from temperature) | `InverseOptimizer::trainStep` iterative loop | `trainStep` function |
| Loss function (MSE) | `computeMSE` computing mean squared error | `computeMSE` |
| Gradient descent | `α -= lr * grad`, combined with RMSprop | Update loop |
| Gradient computation (numerical) | Central difference \((L_{+ε}-L_{-ε})/(2ε)\) | Gradient double loop |
| Optimisation acceleration (RMSprop) | Per‑parameter moving average of `grad_sq_avg` | RMSprop normalisation loop |
| Parallel computation (OpenMP) | `#pragma omp parallel for collapse(2)` | Gradient / update loops |
| Physical constraints (parameter bounds) | `std::clamp(α, 0.001, 1.0)` | Post‑update projection |