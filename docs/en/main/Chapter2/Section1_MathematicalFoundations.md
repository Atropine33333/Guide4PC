# Section 1: Mathematical Foundations

> Sections 1 and 2 of Chapter 2 are the two toughest sections in the entire chapter – they may require knowledge from sophomore to junior year, especially in math. Still, I hope readers will give them a try; I think my explanations are fairly clear. It's a lot like the C and D reading passages in English exams – you probably won't understand all the terminology, but you can roughly get the gist.

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Partial derivative | The derivative of a multivariable function with respect to one variable, holding the others fixed |
| Gradient | A vector composed of all partial derivatives, pointing in the direction of steepest ascent |
| Laplace operator (Δ) | The sum of second partial derivatives in all spatial directions; measures deviation from the local average |
| Thermal diffusivity (α) | A physical parameter describing a material's ability to conduct heat—larger values mean faster heat spreading |
| Heat equation | A partial differential equation that describes how temperature evolves over time and space |
| Finite difference method | Approximates derivatives using differences between discrete grid points, allowing computers to solve PDEs |
| Explicit Euler | The simplest time‑stepping method—computes the next state directly from the current state |
| CFL condition | A numerical stability constraint: the time step must not exceed a threshold determined by the spatial step size and diffusivity |
| Forward problem | Given all parameters and initial conditions, solving for the temperature field |
| Inverse problem | Inferring unknown parameters (thermal diffusivity) from observed temperature data |
| Loss function | A metric that quantifies the mismatch between predicted and observed temperatures—smaller is better |
| Gradient descent | Adjusts parameters in the direction opposite to the gradient of the loss function, reducing the loss iteratively |
| Finite‑difference gradient | Approximates the gradient using two forward simulations (perturbing the parameter by ±ε), eliminating the need for analytic derivatives |
| RMSprop | An improved gradient‑descent algorithm that adapts the learning rate per parameter, making training more stable |
| Multi‑time loss | Compares temperatures at multiple intermediate time steps—not just the final one—improving inversion stability |
| Clipping | Restricts parameters to physically reasonable bounds (e.g., α must be positive and not too large) |

---

## 1.1 From Single‑Variable to Multivariable Functions

The functions studied in high‑school mathematics take the form \(y = f(x)\)—a single independent variable, with the dependent variable changing solely with that one input. In heat‑transfer problems, however, temperature \(T\) depends simultaneously on spatial position \((x, y)\) and time \(t\). We therefore need the concept of a **multivariable function**: \(T = T(x, y, t)\).

For such functions, the rate of change is no longer unique—it may differ depending on which direction you look. This brings us to the definition of the **partial derivative**: the rate of change of the function with respect to one variable, while keeping all other independent variables fixed.

**Formal definition of the partial derivative:**

\[
\frac{\partial T}{\partial x} = \lim_{\Delta x \to 0} \frac{T(x + \Delta x, y, t) - T(x, y, t)}{\Delta x}
\]

The symbol \(\partial\) (read as "partial") distinguishes partial derivatives from ordinary derivatives of single‑variable functions, which use the symbol \(d\). This expression gives the instantaneous rate of change of temperature in the \(x\)-direction, with \(y\) and \(t\) treated as constants.

**Physical meaning of first‑order partial derivatives:**

- \(\frac{\partial T}{\partial x}\): the temperature gradient in the \(x\)-direction (slope)
- \(\frac{\partial T}{\partial y}\): the temperature gradient in the \(y\)-direction
- \(\frac{\partial T}{\partial t}\): the rate at which temperature changes over time

In the actual program, since computers cannot perform limiting operations, partial derivatives are approximated using finite differences (see Section 1.6) as incremental ratios.

---

## 1.2 Second‑Order Partial Derivatives and the Laplace Operator

Taking the partial derivative of a partial derivative gives a **second‑order partial derivative**, for example:

\[
\frac{\partial^2 T}{\partial x^2} = \frac{\partial}{\partial x}\left(\frac{\partial T}{\partial x}\right)
\]

A second‑order partial derivative measures the **curvature** (concavity/convexity) of the function, rather than its slope. In physics, the spatial sum of second‑order partial derivatives reflects how much a given point deviates from its surroundings.

The **Laplace operator** (Laplacian) is defined as the sum of second‑order partial derivatives in all spatial directions:

\[
\nabla^2 T = \frac{\partial^2 T}{\partial x^2} + \frac{\partial^2 T}{\partial y^2}
\]

The symbol \(\nabla^2\) (read as "Laplacian") is a compact operator notation. Physically, the Laplacian tells us whether a point is hotter or cooler than its immediate neighbourhood: if a point is hotter than its surroundings, then \(\nabla^2 T < 0\), and heat flows outward; if it is cooler, then \(\nabla^2 T > 0\), and heat flows inward. This mechanism will be discretised into an explicit arithmetic expression in Section 1.6.

---

## 1.3 Domain and Grid Discretisation

Let the two‑dimensional bounded rectangular domain be \(\Omega = [0, L_x] \times [0, L_y]\), with \(L_x = 1.0\) (metres) and \(L_y = 0.5\) (metres). The continuous scalar field \(T(x, y, t)\) defined on \(\Omega\) represents the temperature distribution at time \(t\).

Digital computers can only handle a finite number of discrete values, so the continuous domain must be partitioned into a **grid**. This program uses a uniform structured grid, with spatial step sizes defined as:

\[
\Delta x = \frac{L_x}{N_x - 1}, \quad \Delta y = \frac{L_y}{N_y - 1}
\]

where \(N_x = 40\) and \(N_y = 20\). The grid point coordinates satisfy:

\[
x_i = i \cdot \Delta x, \quad i \in \{0, 1, \dots, N_x - 1\}
\]
\[
y_j = j \cdot \Delta y, \quad j \in \{0, 1, \dots, N_y - 1\}
\]

The values of the physical quantity \(T\) on the discrete grid are denoted by \(T_{i,j}^{(n)}\), where the subscripts \((i, j)\) indicate spatial location and the superscript \(n\) indicates the time level. This notation transforms the continuous field into a sequence of two‑dimensional arrays that can be stored in computer memory. In the program, this corresponds to the `Grid2D` class, where the `at(i, j)` method accesses the discrete value corresponding to the physical coordinates \((x_i, y_j)\).

---

## 1.4 Defining the Partial Differential Equation

The physical process solved by this program is governed by the following inhomogeneous linear partial differential equation (PDE):

\[
\frac{\partial T}{\partial t} = \alpha(x, y) \left( \frac{\partial^2 T}{\partial x^2} + \frac{\partial^2 T}{\partial y^2} \right) - v_x \frac{\partial T}{\partial x} - v_y \frac{\partial T}{\partial y} + S(x, y) \tag{1}
\]

The key distinction between a PDE and an ordinary differential equation is that the unknown function \(T\) depends on multiple independent variables, and the equation contains partial derivatives. Each term in equation (1) corresponds to a specific physical mechanism. Let's break them down:

1. **Transient term** \(\frac{\partial T}{\partial t}\): the local rate of temperature change over time. If this term vanishes, the system is at steady state (temperature no longer changes with time).

2. **Diffusion term** \(\alpha \nabla^2 T\): the spontaneous conduction of heat from hot regions to cold regions. The coefficient \(\alpha(x, y)\) is the **thermal diffusivity** (units: m²/s), a spatially distributed physical parameter. The magnitude of this term depends on the local curvature of the field—greater curvature means stronger heat flow.

3. **Advection (convection) term** \(-v_x \frac{\partial T}{\partial x} - v_y \frac{\partial T}{\partial y}\): the transport of heat by fluid flow. In this program, we set \(v_x = 0\) and \(v_y = 0\) (pure diffusion, no advection), but the code retains the general structure for future extensions.

4. **Source term** \(S(x, y)\): an external volumetric heat source. In this program, it is defined as a Gaussian function centred at \((0.3, 0.25)\), with amplitude \(500.0\) and standard deviation \(0.05\), modelling a localised heating element.

A well‑posed solution to equation (1) also requires **boundary conditions**. This program uses homogeneous Dirichlet boundary conditions:

\[
T(x, y, t)\big|_{\partial \Omega} = 0, \quad \forall t \ge 0
\]

That is, the temperature on the domain boundary is held at zero at all times. The boundary conditions provide the surrounding‑environment information needed to solve the PDE.

---

## 1.5 Finite‑Difference Discretisation

To transform equation (1) into an iterative computation that a computer can execute, we must replace the partial derivative symbols with algebraic expressions. The core idea of the finite‑difference method is to approximate derivatives using the values of the function at discrete grid points, based on Taylor expansion.

### 1.5.1 Difference Approximations for First‑Order Derivatives

For a function \(f(x)\), forward and backward differences are defined using values at a point and its right‑ or left‑neighbour:

Forward difference (forward Euler approximation):
\[
f'(x) \approx \frac{f(x + h) - f(x)}{h}
\]

Backward difference:
\[
f'(x) \approx \frac{f(x) - f(x - h)}{h}
\]

where \(h\) is the step size. In this program, the advection terms use **upwind differencing**, which chooses between the two schemes based on the direction of the velocity, to maintain numerical stability.

**Upwind decision rule:**
- If \(v_x \ge 0\) (flow from left to right), use backward differencing: \(\frac{\partial T}{\partial x} \approx \frac{T_{i,j} - T_{i-1,j}}{\Delta x}\)
- If \(v_x < 0\) (flow from right to left), use forward differencing: \(\frac{\partial T}{\partial x} \approx \frac{T_{i+1,j} - T_{i,j}}{\Delta x}\)

This rule ensures that the differencing direction aligns with the direction of information propagation, preventing non‑physical oscillations.

### 1.5.2 Difference Approximation for Second‑Order Derivatives (Central Difference)

The central‑difference scheme for a second derivative is derived by averaging the forward and backward first‑order difference approximations:

\[
\frac{\partial^2 T}{\partial x^2} \approx \frac{T_{i-1,j} - 2T_{i,j} + T_{i+1,j}}{\Delta x^2}
\]

This scheme is second‑order accurate, meaning the truncation error is \(O(\Delta x^2)\). Halving the step size reduces the error by a factor of four.

### 1.5.3 Discretising the Laplacian

Combining the second‑order differences in both directions, and assuming \(\Delta x = \Delta y\), gives:

\[
\nabla^2 T_{i,j} \approx \frac{T_{i-1,j} + T_{i+1,j} + T_{i,j-1} + T_{i,j+1} - 4T_{i,j}}{\Delta x^2} \tag{2}
\]

Equation (2) corresponds to the `lap` variable computed inside `HeatSolver::stepForward` in the code. This stencil is also known as the "five‑point difference star", as it only involves four orthogonal neighbours. The coefficient \(-4T_{i,j}\) arises from the combination of terms and reflects the deviation of the centre point from its surrounding average.

---

## 1.6 Explicit Time Marching and Stability Conditions

Equation (1) is discretised in time using the first‑order forward Euler method:

\[
\frac{T_{i,j}^{(n+1)} - T_{i,j}^{(n)}}{\Delta t} = F(T_{i,j}^{(n)})
\]

where \(F\) represents the sum of all spatial terms on the right‑hand side of equation (1). Rearranging gives the **explicit update formula**:

\[
T_{i,j}^{(n+1)} = T_{i,j}^{(n)} + \Delta t \cdot F(T_{i,j}^{(n)}) \tag{3}
\]

This scheme is called "explicit" because the values at the new time level \((n+1)\) are computed directly from known quantities at the current level \((n)\), without solving a system of linear equations. This greatly simplifies implementation but introduces a **stability restriction**.

**The Courant‑Friedrichs‑Lewy (CFL) condition**:

For the diffusion equation, the explicit scheme is stable only if the time step \(\Delta t\) does not exceed a critical value determined by the spatial step size and diffusivity:

\[
\Delta t_{crit} = \frac{\Delta x^2}{4 \cdot \max(\alpha)}
\]

The intuition is that the characteristic propagation speed of the diffusion process is \(\alpha / \Delta x\). Information must not travel more than one grid spacing per time step; otherwise the numerical solution will blow up.

This program uses an actual time step of \(\Delta t = 0.5 \cdot \Delta t_{crit}\), introducing a safety factor of \(0.5\) to account for the additional stiffness that would arise from advection (if present). If \(\Delta t\) exceeds the critical value, the numerical solution will develop oscillations and eventually grow without bound—a behaviour that has no physical counterpart. The program must therefore enforce this condition.

---

## 1.7 The Forward Problem

The forward problem is defined as: given an initial condition \(T_{i,j}^{(0)}\), boundary conditions, the source term \(S_{i,j}\), the velocity field, and the thermal diffusivity field \(\alpha_{i,j}\), iteratively apply equation (3) for \(n_{steps}\) steps to obtain the temperature field \(T_{i,j}^{(N)}\) at the final time.

In this program, a "true" diffusivity \(\alpha_{true}\) (with a value of \(0.1\) on the left region and \(0.8\) on the right) is used to perform an 8000‑step forward simulation. The temperature field is **recorded every 500 steps**, producing a "multi‑time observation sequence". This provides richer information than recording only the final time, and improves the robustness of the inversion.

Mathematically, this process can be viewed as a deterministic mapping:

\[
\mathcal{F}: \alpha_{i,j} \mapsto \{ T_{i,j}^{(t_1)}, T_{i,j}^{(t_2)}, \dots, T_{i,j}^{(t_m)} \}
\]

That is, a mapping from the parameter space to the observation space across multiple time instants.

---

## 1.8 The Inverse Problem and the Loss Function

The motivation for inversion is as follows: suppose we have observed temperature fields at multiple times (in the program, these are generated synthetically from the true diffusivity), but the thermal diffusivity field \(\alpha\) is unknown. We want to infer the spatial distribution of \(\alpha\) from the observed data. This is a classic **ill‑posed inverse problem**, because the forward mapping \(\mathcal{F}\) is generally not injective—different parameter distributions can produce similar observations.

To quantify the mismatch between the predicted sequence and the observed sequence, we introduce a **loss function**. This program uses the Mean Squared Error (MSE), **averaged over all recorded time steps**:

\[
\mathcal{L}(\tilde{\alpha}) = \frac{1}{M} \sum_{m=1}^{M} \left( \frac{1}{N_x \cdot N_y} \sum_{i,j} \left( \tilde{T}_{i,j}^{(t_m)} - T^{obs}_{i,j}^{(t_m)} \right)^2 \right) \tag{4}
\]

where \(M\) is the number of recorded time instants (in this program, 8000/500 = 16). The loss function is a non‑negative scalar that measures the overall deviation between the predicted and observed temperature sequences. \(\mathcal{L}=0\) if and only if the predictions match the observations exactly at all recorded times.

The goal of inversion is to find a parameter configuration that minimises the loss function:

\[
\alpha^* = \arg \min_{\alpha} \mathcal{L}(\alpha) \tag{5}
\]

Since \(\mathcal{L}\) is a complex nonlinear function of \(\alpha\), it cannot be solved analytically and must be optimised iteratively using numerical methods.

---

## 1.9 Gradient Descent

To solve equation (5), we use **Gradient Descent**, the most fundamental derivative‑based iterative optimisation algorithm.

The **gradient** \(\nabla \mathcal{L}\) is a vector field whose components are the partial derivatives of the loss function with respect to each parameter:

\[
\left(\nabla \mathcal{L}\right)_{i,j} = \frac{\partial \mathcal{L}}{\partial \alpha_{i,j}}
\]

Geometrically, the gradient vector points in the direction of steepest ascent of the loss function in the parameter space. Therefore, updating the parameters in the opposite direction to the gradient guarantees the steepest descent of the loss, at least locally.

**Update rule**:

\[
\alpha^{(k+1)}_{i,j} = \alpha^{(k)}_{i,j} - \eta \cdot \frac{\partial \mathcal{L}}{\partial \alpha_{i,j}} \Bigg|_{\alpha^{(k)}} \tag{6}
\]

where:
- \(k\) is the iteration index
- \(\eta\) is the learning rate, controlling the step size per update
- the partial derivative is evaluated at \(\alpha^{(k)}\), i.e., the "current location"

Choosing the learning rate involves balancing convergence speed and stability: too large and the parameters may overshoot the optimum and oscillate; too small and convergence becomes painfully slow.

---

## 1.10 Computing the Gradient: The Finite‑Difference Perturbation Method

The gradient vector must be computed individually for every grid point, i.e., \(N_x \times N_y\) partial derivatives. Since the dependence of \(\mathcal{L}(\alpha)\) on \(\alpha_{i,j}\) is implicitly embedded inside 8000 steps of forward simulation, we cannot write down an analytic derivative expression. Instead, we use numerical differentiation.

**Central finite‑difference perturbation**:

\[
\frac{\partial \mathcal{L}}{\partial \alpha_{i,j}} \approx \frac{\mathcal{L}(\alpha_{i,j} + \varepsilon) - \mathcal{L}(\alpha_{i,j} - \varepsilon)}{2 \varepsilon} \tag{7}
\]

where \(\varepsilon > 0\) is a small positive number (set to \(0.01\) in this program).

The implementation workflow is:
1. Make a copy of the current \(\alpha\) field.
2. Increase the value at grid point \((i, j)\) by \(\varepsilon\), leaving all other points unchanged.
3. Run a full forward simulation (8000 steps), record the temperatures at the specified times, and compute the multi‑time loss \(L_{plus}\).
4. Decrease the value at that point by \(\varepsilon\) (equivalent to subtracting \(\varepsilon\) from the original value), run the forward simulation, and compute the loss \(L_{minus}\).
5. Approximate the partial derivative using equation (7).
6. Restore the original value and move to the next grid point.

**Computational complexity**: a single gradient evaluation requires \(2 \times N_x \times N_y\) full forward simulations. With \(N_x \times N_y = 800\) in this program, that's 1600 forward simulations per gradient, each running 8000 time steps, totalling 12.8 million grid updates. This is the fundamental reason the program uses OpenMP parallelism.

The central‑difference scheme has a truncation error of \(O(\varepsilon^2)\), which is better than forward differencing (error \(O(\varepsilon)\)). The choice of \(\varepsilon\) involves a trade‑off between truncation error (larger \(\varepsilon\) worsens it) and rounding error (smaller \(\varepsilon\) suffers from floating‑point precision loss).

---

## 1.11 RMSprop: Adaptive Learning Rates

During gradient descent, the magnitudes of gradients for different parameters can vary enormously. For example, grid points near the heat source are more sensitive to temperature changes and thus have larger gradients, while points farther away have smaller gradients. Using a single global learning rate can cause parameters with large gradients to oscillate and those with small gradients to converge slowly.

**RMSprop** (Root Mean Square Propagation) maintains a moving average of the squared gradient history for each parameter, allowing it to adjust the learning rate individually:

\[
s_{i,j}^{(k)} = \rho \cdot s_{i,j}^{(k-1)} + (1 - \rho) \cdot \left( \frac{\partial \mathcal{L}}{\partial \alpha_{i,j}} \right)^2
\]
\[
\alpha_{i,j}^{(k+1)} = \alpha_{i,j}^{(k)} - \frac{\eta}{\sqrt{s_{i,j}^{(k)} + \delta}} \cdot \frac{\partial \mathcal{L}}{\partial \alpha_{i,j}}
\]

where:
- \(\rho\) is the decay rate (set to 0.9 in this program), controlling how quickly past information is forgotten
- \(\delta\) is a small constant (set to \(10^{-6}\)) to prevent division by zero
- Each parameter's learning rate is normalised by \(s\): parameters with large gradients get automatically reduced effective learning rates, while those with small gradients get increased rates

This stabilises training and accelerates convergence.

---

## 1.12 Physical Constraints and Parameter Projection

Thermal diffusivity \(\alpha\) has a clear physical meaning: its units are m²/s, and for real materials it must be positive. Furthermore, the stability condition for the numerical scheme (Section 1.6) imposes an upper bound on \(\alpha\)—otherwise \(\Delta t_{crit}\) becomes too small and the computational cost becomes prohibitive.

Therefore, after each gradient update, we apply **clipping**:

\[
\alpha_{i,j} \leftarrow \min\left(\alpha_{\max}, \max(\alpha_{\min}, \alpha_{i,j})\right)
\]

In this program, we set \(\alpha_{\min} = 0.001\) (to prevent numerical singularities) and \(\alpha_{\max} = 1.0\) (to keep the CFL condition manageable). This projection confines the parameters to a physically feasible compact convex set, ensuring both numerical stability and physical plausibility.

---

## 1.13 Mathematical‑to‑Code Cross‑Reference

To directly connect the mathematical concepts above to the actual program code, the table below maps the main mathematical objects to their implementation locations in `thermal.cpp`.

| Mathematical Concept | Corresponding Code Location |
| :--- | :--- |
| Partial derivatives (upwind differencing) | `dTx`, `dTy` inside `HeatSolver::stepForward` |
| Laplacian (five‑point stencil) | `lap` inside `HeatSolver::stepForward` |
| Explicit Euler update | `T.at(i,j) = T_old.at(i,j) + dt * (a * lap - vx * dTx - vy * dTy + S);` |
| CFL condition calculation | `dt_crit` inside `InverseOptimizer::trainStep` |
| Multi‑time loss function | `HeatSolver::computeMSE` and the averaging in `runSimulation` |
| Finite‑difference gradient | The double loops and `Lplus`/`Lminus` inside `InverseOptimizer::trainStep` |
| RMSprop state | `grad_sq_avg` and its update formula |
| Parameter projection | `std::clamp(new_val, 0.001, alpha_max)` |

---

## Suggestions for Further Study

The material above covers all the mathematical tools that this program relies on. I recommend reading the code alongside the text in the following order:

1. Find the grid parameter definitions (`NX`, `NY`, `dx`, `dy`) in `main` and relate them to Section 1.3's discretisation.
2. Find the computations of `lap`, `dTx`, and `dTy` inside `HeatSolver::stepForward` and verify the difference schemes against Section 1.5.
3. Find the double loop for gradient computation and the calls to `runSimulation` inside `InverseOptimizer::trainStep`, and connect them to Section 1.10's perturbation method.
4. Find the update of `grad_sq_avg` and the parameter‑update formula, and relate them to Section 1.11 on RMSprop.
5. Find the `std::clamp` call and relate it to Section 1.12's projection operation.

Once you understand these mathematical foundations, you'll be able to independently adjust the equation parameters, modify the grid resolution, or even replace the PDE with a different one for your own experiments.