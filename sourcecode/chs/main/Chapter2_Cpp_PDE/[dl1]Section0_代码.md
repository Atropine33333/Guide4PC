# 第0节：代码，输出和日志

> 本项目的代码放在蓝奏云上了，至于为什么不放在github上，因为github把我三个账户都封禁了。
> 至于为什么不直接放在网页里下载，因为我还没写这一块，等我写出来就会更新。
> 。

https://wwbnb.lanzouw.com/b009i7k6if
密码:cpppde

---

## 0.1 代码

```cpp
/**
 * @file thermal.cpp
 * @brief 2D transient heat conduction inverse problem using finite-difference
 *        perturbation (DeepFDM philosophy).  Multi‑time loss, RMSprop, and
 *        spatial sensitivity tuning recover a hidden low‑conductivity region.
 *
 * Physics:
 *   ∂T/∂t = α(x,y)·∇²T + S(x,y)   (convection disabled for clarity)
 *
 * Domain: [0,1]×[0,0.5] m,  NX=40, NY=20,  Dirichlet T=0 on all boundaries.
 *
 * TRUE FIELD:  left half  (x < 0.5) → α = 0.1  (insulating air gap)
 *              right half (x ≥ 0.5) → α = 0.8  (copper)
 * Source: Gaussian centred at (0.3,0.25) with amplitude 500.
 *
 * The source lies inside the low‑α region, making the inverse problem well‑posed.
 *
 * Forward solver:  explicit Euler, fixed 3×3 Laplacian kernel, upwind (unused).
 * Inverse solver:  central finite‑difference gradient, RMSprop, multi‑time
 *                  mean‑squared‑error loss, gradient clipping.
 *
 * Compilation:  g++ -std=c++17 -O2 -fopenmp main.cpp -o thermal
 */

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>

// -----------------------------------------------------------------------------
// Global log stream – writes to both console and file "inversion.log"
// -----------------------------------------------------------------------------
std::ofstream g_logfile;  ///< Output file for all logging messages

/**
 * @brief Write a message to both std::cout and the global log file.
 * @param msg  Message string (without trailing newline; a newline is appended).
 */
inline void log_write(const std::string& msg) {
    std::cout << msg << std::endl;
    if (g_logfile.is_open()) g_logfile << msg << std::endl;
}

// =============================================================================
// Class Grid2D : 2D data container (row‑major)
// =============================================================================
class Grid2D {
public:
    int nx, ny;                 ///< Grid dimensions
    std::vector<double> data;   ///< Row‑major storage: index = j * nx + i

    /**
     * @brief Construct a zero‑filled grid.
     * @param nx_  Number of points in x direction
     * @param ny_  Number of points in y direction
     */
    Grid2D(int nx_ = 0, int ny_ = 0) : nx(nx_), ny(ny_), data(nx_ * ny_, 0.0) {}

    /// Element access (i = x-index, j = y-index)
    double& at(int i, int j) { return data[j * nx + i]; }
    /// Const element access
    const double& at(int i, int j) const { return data[j * nx + i]; }

    /**
     * @brief Set all four boundary rows/columns to a constant (Dirichlet BC).
     */
    void setBoundary(double value) {
        for (int i = 0; i < nx; ++i) {
            at(i, 0) = value;          // bottom
            at(i, ny - 1) = value;     // top
        }
        for (int j = 0; j < ny; ++j) {
            at(0, j) = value;          // left
            at(nx - 1, j) = value;     // right
        }
    }

    /// Deep copy from another Grid2D of the same size.
    void copyFrom(const Grid2D& other) {
        std::copy(other.data.begin(), other.data.end(), data.begin());
    }
};

// =============================================================================
// Class HeatSolver : forward PDE engine
// =============================================================================
class HeatSolver {
public:
    double dx, dy, dt;   ///< Spatial steps and current time step

    /**
     * @brief Constructor storing spatial steps and an initial time step.
     */
    HeatSolver(double dx_, double dy_, double dt_)
        : dx(dx_), dy(dy_), dt(dt_) {}

    /// Update the time step (used to satisfy the CFL condition).
    void setDt(double new_dt) { dt = new_dt; }

    /**
     * @brief Advance temperature by one explicit Euler step.
     *
     * **DeepFDM physical prior:**
     * - Laplacian is computed with a **fixed 3×3 kernel** `[[0,1,0],[1,-4,1],[0,1,0]] / dx²`.
     * - Convection uses **upwind** differencing.
     * - A double‑buffer (T_old) guarantees that T^{n+1} depends only on T^n.
     */
    void stepForward(Grid2D& T, const Grid2D& Alpha, const Grid2D& Source,
                     double vx, double vy) {
        Grid2D T_old = T;   // deep copy – essential for explicit Euler

        for (int j = 1; j < T.ny - 1; ++j) {
            for (int i = 1; i < T.nx - 1; ++i) {
                // ---- Fixed Laplacian (physical prior) ----
                double lap = (T_old.at(i-1,j) + T_old.at(i+1,j) +
                              T_old.at(i,j-1) + T_old.at(i,j+1) -
                              4.0 * T_old.at(i,j)) / (dx * dx);

                // ---- Upwind x-gradient ----
                double dTx = (vx >= 0.0)
                                 ? (T_old.at(i,j) - T_old.at(i-1,j)) / dx
                                 : (T_old.at(i+1,j) - T_old.at(i,j)) / dx;
                // ---- Upwind y-gradient ----
                double dTy = (vy >= 0.0)
                                 ? (T_old.at(i,j) - T_old.at(i,j-1)) / dy
                                 : (T_old.at(i,j+1) - T_old.at(i,j)) / dy;

                T.at(i,j) = T_old.at(i,j) +
                            dt * (Alpha.at(i,j) * lap - vx * dTx - vy * dTy + Source.at(i,j));
            }
        }
        T.setBoundary(0.0);     // Enforce Dirichlet BCs after each step
    }

    /** Standard mean squared error between two fields. */
    static double computeMSE(const Grid2D& a, const Grid2D& b) {
        double sum = 0.0;
        for (size_t k = 0; k < a.data.size(); ++k) {
            double diff = a.data[k] - b.data[k];
            sum += diff * diff;
        }
        return sum / static_cast<double>(a.data.size());
    }
};

// =============================================================================
// Class InverseOptimizer : gradient‑based inversion
// =============================================================================
class InverseOptimizer {
public:
    HeatSolver* solver_template;   ///< Template solver for cloning
    double lr;                     ///< Learning rate
    double eps;                    ///< Finite‑difference perturbation
    double alpha_max;              ///< Upper bound for alpha (clamp)
    double rho;                    ///< RMSprop decay rate
    double eps_rms;                ///< RMSprop epsilon (avoids division by zero)
    int sim_steps;                 ///< Total forward time steps per simulation
    int record_every;              ///< Record temperature every this many steps
    std::vector<Grid2D>* target_records; ///< Multi‑time ground truth

    Grid2D grad_sq_avg;            ///< RMSprop moving average of squared gradients

    /**
     * @brief Constructor.
     * @param solver        Pointer to a HeatSolver whose dx,dy are used for cloning.
     * @param learning_rate Step size (applied after RMSprop normalization).
     * @param perturb_eps   Perturbation magnitude for finite differences.
     * @param steps         Number of forward Euler steps per simulation.
     * @param alpha_max_    Maximum allowed value of alpha.
     * @param rho_val       RMSprop decay rate (default 0.9).
     * @param eps_val       RMSprop epsilon (default 1e-6).
     * @param rec_every     Recording interval for multi‑time loss.
     * @param tgt_rec       Pointer to a vector of recorded target temperature fields.
     */
    InverseOptimizer(HeatSolver* solver, double learning_rate, double perturb_eps,
                     int steps, double alpha_max_,
                     double rho_val = 0.9, double eps_val = 1e-6,
                     int rec_every = 500, std::vector<Grid2D>* tgt_rec = nullptr)
        : solver_template(solver), lr(learning_rate), eps(perturb_eps),
          alpha_max(alpha_max_), rho(rho_val), eps_rms(eps_val),
          sim_steps(steps), record_every(rec_every), target_records(tgt_rec) {}

    /**
     * @brief Perform one training step: gradient computation, RMSprop,
     *        gradient clipping, and parameter update.
     *
     * **DeepFDM core:**
     * - Gradient is obtained by central finite differences – **no automatic
     *   differentiation**.
     * - All 800 grid points are processed in parallel with OpenMP.
     * - RMSprop normalizes the update per parameter, balancing regions with
     *   very different sensitivities.
     * - After normalization, the gradient is clipped element‑wise to [-1,1]
     *   to avoid occasional numerical spikes.
     * - Alpha is clamped to [0.001, alpha_max] after each update.
     */
    void trainStep(Grid2D& Alpha, const Grid2D& Source,
                   const Grid2D& T_init, double vx, double vy) {
        int nx = Alpha.nx, ny = Alpha.ny;

        // ---- CFL‑safe time step (0.5 × critical) ----
        double max_a = *std::max_element(Alpha.data.begin(), Alpha.data.end());
        double dt_crit = solver_template->dx * solver_template->dx / (4.0 * max_a);
        double dt_safe = 0.5 * dt_crit;
        solver_template->setDt(dt_safe);

        // ---- Lazy initialization of RMSprop state ----
        if (grad_sq_avg.nx == 0) {
            grad_sq_avg = Grid2D(nx, ny);
            std::fill(grad_sq_avg.data.begin(), grad_sq_avg.data.end(), 0.0);
        }

        Grid2D grad(nx, ny);
        std::fill(grad.data.begin(), grad.data.end(), 0.0);

        Grid2D Alpha_backup(nx, ny);
        Alpha_backup.copyFrom(Alpha);

        // ====== Parallel finite‑difference gradient computation ======
        #pragma omp parallel
        {
            HeatSolver local_solver(solver_template->dx, solver_template->dy, dt_safe);
            Grid2D T_pred(nx, ny);
            Grid2D Alpha_pert(nx, ny);
            Alpha_pert.copyFrom(Alpha_backup);

            #pragma omp for collapse(2) schedule(dynamic)
            for (int j = 0; j < ny; ++j) {
                for (int i = 0; i < nx; ++i) {
                    double orig = Alpha_backup.at(i,j);

                    // +ε
                    Alpha_pert.at(i,j) = orig + eps;
                    double Lplus = runSimulation(T_pred, Alpha_pert, Source,
                                                 vx, vy, local_solver, T_init);
                    // -ε
                    Alpha_pert.at(i,j) = orig - eps;
                    double Lminus = runSimulation(T_pred, Alpha_pert, Source,
                                                  vx, vy, local_solver, T_init);

                    Alpha_pert.at(i,j) = orig;   // restore
                    grad.at(i,j) = (Lplus - Lminus) / (2.0 * eps);
                }
            }
        }

        // ---- Diagnostic: raw gradient statistics ----
        double gmax = *std::max_element(grad.data.begin(), grad.data.end());
        double gmin = *std::min_element(grad.data.begin(), grad.data.end());
        double gavg = 0.0;
        for (double g : grad.data) gavg += std::abs(g);
        gavg /= grad.data.size();
        double g_mon = grad.at(8, 9);   // point inside the low‑α region
        {
            std::ostringstream oss;
            oss << std::scientific << std::setprecision(6);
            oss << "    [Grad] min=" << gmin << " max=" << gmax
                << " avg|grad|=" << gavg << " g(8,9)=" << g_mon;
            log_write(oss.str());
        }

        // ---- RMSprop normalization ----
        #pragma omp parallel for collapse(2)
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                double g = grad.at(i,j);
                double& avg = grad_sq_avg.at(i,j);
                avg = rho * avg + (1.0 - rho) * g * g;
                grad.at(i,j) = g / (std::sqrt(avg) + eps_rms);
            }
        }

        // ---- Gradient clipping (element‑wise to [-1, 1]) ----
        #pragma omp parallel for collapse(2)
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                double& g = grad.at(i,j);
                g = std::clamp(g, -1.0, 1.0);
            }
        }

        // ---- Parameter update + physical clamping ----
        #pragma omp parallel for collapse(2)
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                double new_val = Alpha.at(i,j) - lr * grad.at(i,j);
                new_val = std::clamp(new_val, 0.001, alpha_max);
                Alpha.at(i,j) = new_val;
            }
        }
    }

private:
    /**
     * @brief Run a full forward simulation and return the averaged multi‑time MSE loss.
     *
     * The temperature is recorded every `record_every` steps and compared
     * against the corresponding entry in `target_records`.  The returned value
     * is the arithmetic mean over all recorded times.
     */
    double runSimulation(Grid2D& T_pred, const Grid2D& Alpha, const Grid2D& Source,
                         double vx, double vy, HeatSolver& solver,
                         const Grid2D& T_init) {
        T_pred.copyFrom(T_init);
        T_pred.setBoundary(0.0);
        double loss_sum = 0.0;
        int rec_idx = 0;
        for (int k = 1; k <= sim_steps; ++k) {
            solver.stepForward(T_pred, Alpha, Source, vx, vy);
            if (k % record_every == 0 && rec_idx < static_cast<int>(target_records->size())) {
                loss_sum += HeatSolver::computeMSE(T_pred, (*target_records)[rec_idx]);
                ++rec_idx;
            }
        }
        return (rec_idx > 0) ? loss_sum / rec_idx : 0.0;
    }
};

// -----------------------------------------------------------------------------
// Write a Grid2D to CSV file
// -----------------------------------------------------------------------------
void writeCSV(const std::string& fname, const Grid2D& g) {
    std::ofstream f(fname);
    if (!f) {
        log_write("Error opening " + fname + " for writing.");
        return;
    }
    f << std::fixed << std::setprecision(8);
    for (int j = 0; j < g.ny; ++j) {
        for (int i = 0; i < g.nx; ++i) {
            f << g.at(i,j);
            f << (i < g.nx-1 ? "," : "\n");
        }
    }
    log_write("  -> " + fname + " written");
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
int main() {
    // Open log file
    g_logfile.open("inversion.log");
    log_write("=== DeepFDM 2D thermal inversion started ===");

    const int NX = 40, NY = 20;
    const double Lx = 1.0, Ly = 0.5;
    const double dx = Lx/(NX-1), dy = Ly/(NY-1);
    const double vx = 0.0, vy = 0.0;

    // ---- True alpha field: left low (0.1), right high (0.8) ----
    Grid2D Alpha_true(NX, NY);
    for (int j = 0; j < NY; ++j)
        for (int i = 0; i < NX; ++i)
            Alpha_true.at(i,j) = (i*dx < 0.5) ? 0.1 : 0.8;

    // ---- Gaussian heat source (strong, in the low‑α region) ----
    Grid2D Source(NX, NY);
    const double src_amp = 500.0, src_sigma = 0.05, src_x0 = 0.3, src_y0 = 0.25;
    for (int j = 0; j < NY; ++j) {
        double y = j * dy;
        for (int i = 0; i < NX; ++i) {
            double x = i * dx;
            double r2 = (x - src_x0)*(x - src_x0) + (y - src_y0)*(y - src_y0);
            Source.at(i,j) = src_amp * std::exp(-r2 / (2.0 * src_sigma * src_sigma));
        }
    }

    Grid2D T_init(NX, NY);
    T_init.setBoundary(0.0);

    // ---- Generate multi‑time ground truth ----
    HeatSolver solver(dx, dy, 0.0);
    double max_a = *std::max_element(Alpha_true.data.begin(), Alpha_true.data.end());
    double dt_true = 0.5 * (dx*dx) / (4.0 * max_a);
    solver.setDt(dt_true);
    const int steps_target = 8000;
    const int record_every = 500;
    std::vector<Grid2D> target_records;
    Grid2D T = T_init;
    for (int k = 1; k <= steps_target; ++k) {
        solver.stepForward(T, Alpha_true, Source, vx, vy);
        if (k % record_every == 0)
            target_records.push_back(T);
    }
    {
        std::ostringstream oss;
        oss << "Ground truth generated, " << target_records.size()
            << " records, dt = " << dt_true
            << ", physical time = " << dt_true * steps_target << " s";
        log_write(oss.str());
    }
    writeCSV("alpha_true.csv", Alpha_true);
    writeCSV("temp_target.csv", T);   // final state for plotting

    // ---- Initial guess ----
    Grid2D Alpha_guess(NX, NY);
    std::fill(Alpha_guess.data.begin(), Alpha_guess.data.end(), 0.5);

    // ---- Optimizer (lower lr, gradient clipping, multi‑time loss) ----
    InverseOptimizer optim(&solver,
                           0.002,   // learning rate (reduced for smooth convergence)
                           0.01,    // finite‑difference epsilon
                           steps_target,
                           1.0,     // alpha_max
                           0.9,     // rho
                           1e-6,    // eps_rms
                           record_every,
                           &target_records);
    const int train_iters = 200;
    {
        std::ostringstream oss;
        oss << "\nStarting inversion (" << train_iters
            << " iterations, lr=0.002, gradient clipping, "
            << omp_get_max_threads() << " threads)";
        log_write(oss.str());
    }

    for (int iter = 1; iter <= train_iters; ++iter) {
        optim.trainStep(Alpha_guess, Source, T_init, vx, vy);

        // Report every 10 iterations
        if (iter % 10 == 0 || iter == 1) {
            double cur_max = *std::max_element(Alpha_guess.data.begin(), Alpha_guess.data.end());
            double dt_cur = 0.5 * (dx*dx) / (4.0 * cur_max);
            solver.setDt(dt_cur);
            Grid2D T_pred = T_init;
            double rep_loss = 0.0;
            int rep_idx = 0;
            for (int k = 1; k <= steps_target; ++k) {
                solver.stepForward(T_pred, Alpha_guess, Source, vx, vy);
                if (k % record_every == 0 && rep_idx < (int)target_records.size())
                    rep_loss += HeatSolver::computeMSE(T_pred, target_records[rep_idx++]);
            }
            rep_loss /= rep_idx;
            double amin = *std::min_element(Alpha_guess.data.begin(), Alpha_guess.data.end());
            double amax = *std::max_element(Alpha_guess.data.begin(), Alpha_guess.data.end());
            double left_alpha = Alpha_guess.at(8, 9);

            std::ostringstream oss;
            oss << "Iter " << std::setw(4) << iter
                << "  Loss: " << std::scientific << std::setprecision(6) << rep_loss
                << "  α ∈ [" << std::fixed << std::setprecision(6) << amin
                << ", " << amax << "]"
                << "  Left(8,9)=" << left_alpha;
            log_write(oss.str());
        }
    }

    writeCSV("alpha_inferred.csv", Alpha_guess);
    log_write("\nInversion finished. Compare alpha_true.csv and alpha_inferred.csv.");
    log_write("If left half shows low conductivity (≈0.1) and right half high (≈0.8), the DeepFDM logic succeeded.");

    g_logfile.close();
    return 0;
}
```

---

## 0.2 日志

```log
=== DeepFDM 2D thermal inversion started ===
Ground truth generated, 16 records, dt = 0.000102728, physical time = 0.821828 s
  -> alpha_true.csv written
  -> temp_target.csv written

Starting inversion (200 iterations, lr=0.002, gradient clipping, 16 threads)
    [Grad] min=-9.677091e-05 max=7.652657e-01 avg|grad|=2.102568e-02 g(8,9)=1.013576e-01
Iter    1  Loss: 1.495626e+01  α ∈ [0.498000, 0.502000]  Left(8,9)=0.498000
    [Grad] min=-9.718137e-05 max=7.704842e-01 avg|grad|=2.116831e-02 g(8,9)=1.020438e-01
    [Grad] min=-9.759789e-05 max=7.757547e-01 avg|grad|=2.131235e-02 g(8,9)=1.027368e-01
    [Grad] min=-9.802067e-05 max=7.810779e-01 avg|grad|=2.145782e-02 g(8,9)=1.034366e-01
    [Grad] min=-9.844989e-05 max=7.864545e-01 avg|grad|=2.160474e-02 g(8,9)=1.041433e-01
    [Grad] min=-9.888560e-05 max=7.918851e-01 avg|grad|=2.175311e-02 g(8,9)=1.048570e-01
    [Grad] min=-9.932779e-05 max=7.973705e-01 avg|grad|=2.190297e-02 g(8,9)=1.055778e-01
    [Grad] min=-9.977651e-05 max=8.029113e-01 avg|grad|=2.205433e-02 g(8,9)=1.063058e-01
    [Grad] min=-1.002319e-04 max=8.085084e-01 avg|grad|=2.220720e-02 g(8,9)=1.070411e-01
    [Grad] min=-1.006938e-04 max=8.141623e-01 avg|grad|=2.236162e-02 g(8,9)=1.077838e-01
Iter   10  Loss: 1.464294e+01  α ∈ [0.480000, 0.520000]  Left(8,9)=0.480000
    [Grad] min=-1.011616e-04 max=8.198739e-01 avg|grad|=2.251759e-02 g(8,9)=1.085339e-01
    [Grad] min=-1.016349e-04 max=8.256438e-01 avg|grad|=2.267515e-02 g(8,9)=1.092916e-01
    [Grad] min=-1.021137e-04 max=8.314730e-01 avg|grad|=2.283431e-02 g(8,9)=1.100569e-01
    [Grad] min=-1.025979e-04 max=8.373620e-01 avg|grad|=2.299508e-02 g(8,9)=1.108300e-01
    [Grad] min=-1.030875e-04 max=8.433118e-01 avg|grad|=2.315750e-02 g(8,9)=1.116109e-01
    [Grad] min=-1.035823e-04 max=8.493231e-01 avg|grad|=2.332158e-02 g(8,9)=1.123998e-01
    [Grad] min=-1.040825e-04 max=8.553967e-01 avg|grad|=2.348734e-02 g(8,9)=1.131968e-01
    [Grad] min=-1.045879e-04 max=8.615335e-01 avg|grad|=2.365480e-02 g(8,9)=1.140019e-01
    [Grad] min=-1.050985e-04 max=8.677342e-01 avg|grad|=2.382400e-02 g(8,9)=1.148153e-01
    [Grad] min=-1.056142e-04 max=8.739998e-01 avg|grad|=2.399494e-02 g(8,9)=1.156370e-01
Iter   20  Loss: 1.427105e+01  α ∈ [0.460000, 0.540000]  Left(8,9)=0.460000
    [Grad] min=-1.061346e-04 max=8.803310e-01 avg|grad|=2.416766e-02 g(8,9)=1.164672e-01
    [Grad] min=-1.066592e-04 max=8.867288e-01 avg|grad|=2.434219e-02 g(8,9)=1.173060e-01
    [Grad] min=-1.071878e-04 max=8.931941e-01 avg|grad|=2.451853e-02 g(8,9)=1.181536e-01
    [Grad] min=-1.077201e-04 max=8.997276e-01 avg|grad|=2.469673e-02 g(8,9)=1.190099e-01
    [Grad] min=-1.082558e-04 max=9.063305e-01 avg|grad|=2.487681e-02 g(8,9)=1.198751e-01
    [Grad] min=-1.087948e-04 max=9.130035e-01 avg|grad|=2.505877e-02 g(8,9)=1.207494e-01
    [Grad] min=-1.093368e-04 max=9.197476e-01 avg|grad|=2.524265e-02 g(8,9)=1.216329e-01
    [Grad] min=-1.098825e-04 max=9.265638e-01 avg|grad|=2.542847e-02 g(8,9)=1.225256e-01
    [Grad] min=-1.104329e-04 max=9.334530e-01 avg|grad|=2.561626e-02 g(8,9)=1.234278e-01
    [Grad] min=-1.109882e-04 max=9.404163e-01 avg|grad|=2.580605e-02 g(8,9)=1.243394e-01
Iter   30  Loss: 1.387156e+01  α ∈ [0.440000, 0.560000]  Left(8,9)=0.440000
    [Grad] min=-1.115492e-04 max=9.474546e-01 avg|grad|=2.599786e-02 g(8,9)=1.252607e-01
    [Grad] min=-1.121173e-04 max=9.545689e-01 avg|grad|=2.619171e-02 g(8,9)=1.261918e-01
    [Grad] min=-1.126925e-04 max=9.617602e-01 avg|grad|=2.638764e-02 g(8,9)=1.271328e-01
    [Grad] min=-1.134030e-04 max=9.690297e-01 avg|grad|=2.658567e-02 g(8,9)=1.280838e-01
    [Grad] min=-1.144100e-04 max=9.763784e-01 avg|grad|=2.678582e-02 g(8,9)=1.290450e-01
    [Grad] min=-1.154234e-04 max=9.838073e-01 avg|grad|=2.698814e-02 g(8,9)=1.300165e-01
    [Grad] min=-1.164434e-04 max=9.913175e-01 avg|grad|=2.719264e-02 g(8,9)=1.309984e-01
    [Grad] min=-1.174700e-04 max=9.989102e-01 avg|grad|=2.739936e-02 g(8,9)=1.319908e-01
    [Grad] min=-1.185032e-04 max=1.006586e+00 avg|grad|=2.760833e-02 g(8,9)=1.329940e-01
    [Grad] min=-1.195430e-04 max=1.014347e+00 avg|grad|=2.781956e-02 g(8,9)=1.340080e-01
Iter   40  Loss: 1.344144e+01  α ∈ [0.420000, 0.580000]  Left(8,9)=0.420000
    [Grad] min=-1.205894e-04 max=1.022194e+00 avg|grad|=2.803310e-02 g(8,9)=1.350331e-01
    [Grad] min=-1.216425e-04 max=1.030128e+00 avg|grad|=2.824898e-02 g(8,9)=1.360692e-01
    [Grad] min=-1.227023e-04 max=1.038150e+00 avg|grad|=2.846722e-02 g(8,9)=1.371166e-01
    [Grad] min=-1.237694e-04 max=1.046262e+00 avg|grad|=2.868786e-02 g(8,9)=1.381755e-01
    [Grad] min=-1.249763e-04 max=1.054464e+00 avg|grad|=2.891094e-02 g(8,9)=1.392460e-01
    [Grad] min=-1.262082e-04 max=1.062758e+00 avg|grad|=2.913648e-02 g(8,9)=1.403281e-01
    [Grad] min=-1.274477e-04 max=1.071145e+00 avg|grad|=2.936452e-02 g(8,9)=1.414222e-01
    [Grad] min=-1.286948e-04 max=1.079627e+00 avg|grad|=2.959509e-02 g(8,9)=1.425283e-01
    [Grad] min=-1.299492e-04 max=1.088205e+00 avg|grad|=2.982822e-02 g(8,9)=1.436466e-01
    [Grad] min=-1.312110e-04 max=1.096879e+00 avg|grad|=3.006395e-02 g(8,9)=1.447773e-01
Iter   50  Loss: 1.297722e+01  α ∈ [0.400000, 0.600000]  Left(8,9)=0.400000
    [Grad] min=-1.324801e-04 max=1.105652e+00 avg|grad|=3.030231e-02 g(8,9)=1.459206e-01
    [Grad] min=-1.337562e-04 max=1.114525e+00 avg|grad|=3.054334e-02 g(8,9)=1.470765e-01
    [Grad] min=-1.350393e-04 max=1.123499e+00 avg|grad|=3.078707e-02 g(8,9)=1.482453e-01
    [Grad] min=-1.363293e-04 max=1.132575e+00 avg|grad|=3.103354e-02 g(8,9)=1.494271e-01
    [Grad] min=-1.376260e-04 max=1.141756e+00 avg|grad|=3.128278e-02 g(8,9)=1.506221e-01
    [Grad] min=-1.389290e-04 max=1.151042e+00 avg|grad|=3.153484e-02 g(8,9)=1.518305e-01
    [Grad] min=-1.402379e-04 max=1.160434e+00 avg|grad|=3.178977e-02 g(8,9)=1.530524e-01
    [Grad] min=-1.415523e-04 max=1.169935e+00 avg|grad|=3.204761e-02 g(8,9)=1.542881e-01
    [Grad] min=-1.428717e-04 max=1.179546e+00 avg|grad|=3.230838e-02 g(8,9)=1.555376e-01
    [Grad] min=-1.441959e-04 max=1.189268e+00 avg|grad|=3.257212e-02 g(8,9)=1.568013e-01
Iter   60  Loss: 1.247495e+01  α ∈ [0.380000, 0.620000]  Left(8,9)=0.380000
    [Grad] min=-1.455245e-04 max=1.199103e+00 avg|grad|=3.283887e-02 g(8,9)=1.580792e-01
    [Grad] min=-1.468571e-04 max=1.209053e+00 avg|grad|=3.310868e-02 g(8,9)=1.593716e-01
    [Grad] min=-1.481938e-04 max=1.219118e+00 avg|grad|=3.338159e-02 g(8,9)=1.606787e-01
    [Grad] min=-1.495345e-04 max=1.229301e+00 avg|grad|=3.365762e-02 g(8,9)=1.620005e-01
    [Grad] min=-1.508801e-04 max=1.239603e+00 avg|grad|=3.393682e-02 g(8,9)=1.633374e-01
    [Grad] min=-1.522341e-04 max=1.250026e+00 avg|grad|=3.421923e-02 g(8,9)=1.646896e-01
    [Grad] min=-1.535965e-04 max=1.260571e+00 avg|grad|=3.450488e-02 g(8,9)=1.660571e-01
    [Grad] min=-1.549668e-04 max=1.271240e+00 avg|grad|=3.479383e-02 g(8,9)=1.674402e-01
    [Grad] min=-1.563451e-04 max=1.282035e+00 avg|grad|=3.508611e-02 g(8,9)=1.688392e-01
    [Grad] min=-1.577310e-04 max=1.292958e+00 avg|grad|=3.538177e-02 g(8,9)=1.702541e-01
Iter   70  Loss: 1.193013e+01  α ∈ [0.360000, 0.640000]  Left(8,9)=0.360000
    [Grad] min=-1.591242e-04 max=1.304010e+00 avg|grad|=3.568085e-02 g(8,9)=1.716852e-01
    [Grad] min=-1.605245e-04 max=1.315192e+00 avg|grad|=3.598339e-02 g(8,9)=1.731328e-01
    [Grad] min=-1.619312e-04 max=1.326508e+00 avg|grad|=3.628949e-02 g(8,9)=1.745969e-01
    [Grad] min=-1.633436e-04 max=1.337958e+00 avg|grad|=3.659916e-02 g(8,9)=1.760778e-01
    [Grad] min=-1.647610e-04 max=1.349544e+00 avg|grad|=3.691242e-02 g(8,9)=1.775758e-01
    [Grad] min=-1.661827e-04 max=1.361269e+00 avg|grad|=3.722933e-02 g(8,9)=1.790910e-01
    [Grad] min=-1.676082e-04 max=1.373133e+00 avg|grad|=3.754993e-02 g(8,9)=1.806236e-01
    [Grad] min=-1.690371e-04 max=1.385139e+00 avg|grad|=3.787428e-02 g(8,9)=1.821739e-01
    [Grad] min=-1.704692e-04 max=1.397288e+00 avg|grad|=3.820243e-02 g(8,9)=1.837420e-01
    [Grad] min=-1.719047e-04 max=1.409584e+00 avg|grad|=3.853442e-02 g(8,9)=1.853282e-01
Iter   80  Loss: 1.133761e+01  α ∈ [0.340000, 0.660000]  Left(8,9)=0.340000
    [Grad] min=-1.733459e-04 max=1.422026e+00 avg|grad|=3.887027e-02 g(8,9)=1.869327e-01
    [Grad] min=-1.747965e-04 max=1.434618e+00 avg|grad|=3.921005e-02 g(8,9)=1.885556e-01
    [Grad] min=-1.800419e-04 max=1.447361e+00 avg|grad|=3.955381e-02 g(8,9)=1.901973e-01
    [Grad] min=-1.862489e-04 max=1.460257e+00 avg|grad|=3.990160e-02 g(8,9)=1.918579e-01
    [Grad] min=-1.924313e-04 max=1.473308e+00 avg|grad|=4.025350e-02 g(8,9)=1.935376e-01
    [Grad] min=-1.985892e-04 max=1.486516e+00 avg|grad|=4.060952e-02 g(8,9)=1.952367e-01
    [Grad] min=-2.047241e-04 max=1.499882e+00 avg|grad|=4.096975e-02 g(8,9)=1.969553e-01
    [Grad] min=-2.108362e-04 max=1.513410e+00 avg|grad|=4.133421e-02 g(8,9)=1.986937e-01
    [Grad] min=-2.169254e-04 max=1.527100e+00 avg|grad|=4.170292e-02 g(8,9)=2.004521e-01
    [Grad] min=-2.229921e-04 max=1.540955e+00 avg|grad|=4.207593e-02 g(8,9)=2.022307e-01
Iter   90  Loss: 1.069157e+01  α ∈ [0.320000, 0.680000]  Left(8,9)=0.320000
    [Grad] min=-2.290373e-04 max=1.554977e+00 avg|grad|=4.245329e-02 g(8,9)=2.040296e-01
    [Grad] min=-2.350624e-04 max=1.569167e+00 avg|grad|=4.283504e-02 g(8,9)=2.058492e-01
    [Grad] min=-2.410765e-04 max=1.583527e+00 avg|grad|=4.322123e-02 g(8,9)=2.076896e-01
    [Grad] min=-2.470811e-04 max=1.598060e+00 avg|grad|=4.361190e-02 g(8,9)=2.095509e-01
    [Grad] min=-2.530773e-04 max=1.612768e+00 avg|grad|=4.400709e-02 g(8,9)=2.114334e-01
    [Grad] min=-2.590665e-04 max=1.627651e+00 avg|grad|=4.440684e-02 g(8,9)=2.133373e-01
    [Grad] min=-2.650487e-04 max=1.642713e+00 avg|grad|=4.481121e-02 g(8,9)=2.152627e-01
    [Grad] min=-2.710237e-04 max=1.657954e+00 avg|grad|=4.522022e-02 g(8,9)=2.172098e-01
    [Grad] min=-2.769914e-04 max=1.673377e+00 avg|grad|=4.563393e-02 g(8,9)=2.191787e-01
    [Grad] min=-2.859977e-04 max=1.688984e+00 avg|grad|=4.605237e-02 g(8,9)=2.211697e-01
Iter  100  Loss: 9.985484e+00  α ∈ [0.300000, 0.700000]  Left(8,9)=0.300000
    [Grad] min=-2.953375e-04 max=1.704776e+00 avg|grad|=4.647560e-02 g(8,9)=2.231830e-01
    [Grad] min=-3.046329e-04 max=1.720755e+00 avg|grad|=4.690369e-02 g(8,9)=2.252186e-01
    [Grad] min=-3.138831e-04 max=1.736922e+00 avg|grad|=4.733662e-02 g(8,9)=2.272767e-01
    [Grad] min=-3.230869e-04 max=1.753280e+00 avg|grad|=4.777443e-02 g(8,9)=2.293574e-01
    [Grad] min=-3.322434e-04 max=1.769829e+00 avg|grad|=4.821717e-02 g(8,9)=2.314609e-01
    [Grad] min=-3.413511e-04 max=1.786572e+00 avg|grad|=4.866489e-02 g(8,9)=2.335874e-01
    [Grad] min=-3.504082e-04 max=1.803510e+00 avg|grad|=4.911766e-02 g(8,9)=2.357368e-01
    [Grad] min=-3.594130e-04 max=1.820643e+00 avg|grad|=4.957544e-02 g(8,9)=2.379093e-01
    [Grad] min=-3.683638e-04 max=1.837974e+00 avg|grad|=5.003826e-02 g(8,9)=2.401049e-01
    [Grad] min=-3.772599e-04 max=1.855503e+00 avg|grad|=5.050622e-02 g(8,9)=2.423238e-01
Iter  110  Loss: 9.212132e+00  α ∈ [0.280000, 0.720000]  Left(8,9)=0.280000
    [Grad] min=-3.860995e-04 max=1.873232e+00 avg|grad|=5.097923e-02 g(8,9)=2.445658e-01
    [Grad] min=-3.948808e-04 max=1.891161e+00 avg|grad|=5.145729e-02 g(8,9)=2.468310e-01
    [Grad] min=-4.036023e-04 max=1.909291e+00 avg|grad|=5.194043e-02 g(8,9)=2.491195e-01
    [Grad] min=-4.122649e-04 max=1.927622e+00 avg|grad|=5.242863e-02 g(8,9)=2.514311e-01
    [Grad] min=-4.208707e-04 max=1.946156e+00 avg|grad|=5.292190e-02 g(8,9)=2.537659e-01
    [Grad] min=-4.294245e-04 max=1.964892e+00 avg|grad|=5.342025e-02 g(8,9)=2.561237e-01
    [Grad] min=-4.379252e-04 max=1.983831e+00 avg|grad|=5.392371e-02 g(8,9)=2.585044e-01
    [Grad] min=-4.463804e-04 max=2.002973e+00 avg|grad|=5.443220e-02 g(8,9)=2.609078e-01
    [Grad] min=-4.547908e-04 max=2.022316e+00 avg|grad|=5.494567e-02 g(8,9)=2.633338e-01
    [Grad] min=-4.631533e-04 max=2.041861e+00 avg|grad|=5.546411e-02 g(8,9)=2.657822e-01
Iter  120  Loss: 8.363786e+00  α ∈ [0.260000, 0.740000]  Left(8,9)=0.260000
    [Grad] min=-4.714648e-04 max=2.061607e+00 avg|grad|=5.598747e-02 g(8,9)=2.682528e-01
    [Grad] min=-4.797217e-04 max=2.081553e+00 avg|grad|=5.651570e-02 g(8,9)=2.707451e-01
    [Grad] min=-4.879202e-04 max=2.101696e+00 avg|grad|=5.704875e-02 g(8,9)=2.732590e-01
    [Grad] min=-4.960566e-04 max=2.122036e+00 avg|grad|=5.758655e-02 g(8,9)=2.757940e-01
    [Grad] min=-5.041299e-04 max=2.142571e+00 avg|grad|=5.812901e-02 g(8,9)=2.783497e-01
    [Grad] min=-5.121353e-04 max=2.163297e+00 avg|grad|=5.867617e-02 g(8,9)=2.809255e-01
    [Grad] min=-5.200669e-04 max=2.184212e+00 avg|grad|=5.922796e-02 g(8,9)=2.835210e-01
    [Grad] min=-5.279183e-04 max=2.205312e+00 avg|grad|=5.978426e-02 g(8,9)=2.861356e-01
    [Grad] min=-5.356830e-04 max=2.226594e+00 avg|grad|=6.034484e-02 g(8,9)=2.887685e-01
    [Grad] min=-5.433548e-04 max=2.248053e+00 avg|grad|=6.090952e-02 g(8,9)=2.914190e-01
Iter  130  Loss: 7.432719e+00  α ∈ [0.240000, 0.760000]  Left(8,9)=0.240000
    [Grad] min=-5.509284e-04 max=2.269684e+00 avg|grad|=6.147812e-02 g(8,9)=2.940863e-01
    [Grad] min=-5.583994e-04 max=2.291480e+00 avg|grad|=6.205054e-02 g(8,9)=2.967694e-01
    [Grad] min=-5.657648e-04 max=2.313437e+00 avg|grad|=6.262661e-02 g(8,9)=2.994673e-01
    [Grad] min=-5.730229e-04 max=2.335546e+00 avg|grad|=6.320602e-02 g(8,9)=3.021789e-01
    [Grad] min=-5.801933e-04 max=2.357799e+00 avg|grad|=6.378851e-02 g(8,9)=3.049028e-01
    [Grad] min=-5.872720e-04 max=2.380188e+00 avg|grad|=6.437382e-02 g(8,9)=3.076378e-01
    [Grad] min=-5.942527e-04 max=2.402703e+00 avg|grad|=6.496166e-02 g(8,9)=3.103822e-01
    [Grad] min=-6.086704e-04 max=2.425333e+00 avg|grad|=6.555171e-02 g(8,9)=3.131344e-01
    [Grad] min=-6.505135e-04 max=2.448066e+00 avg|grad|=6.614397e-02 g(8,9)=3.158927e-01
    [Grad] min=-6.913613e-04 max=2.470888e+00 avg|grad|=6.673804e-02 g(8,9)=3.186551e-01
Iter  140  Loss: 6.412246e+00  α ∈ [0.220000, 0.780000]  Left(8,9)=0.220000
    [Grad] min=-7.312137e-04 max=2.493786e+00 avg|grad|=6.733343e-02 g(8,9)=3.214195e-01
    [Grad] min=-7.700613e-04 max=2.516743e+00 avg|grad|=6.792941e-02 g(8,9)=3.241836e-01
    [Grad] min=-8.078910e-04 max=2.539742e+00 avg|grad|=6.852547e-02 g(8,9)=3.269449e-01
    [Grad] min=-8.446886e-04 max=2.562763e+00 avg|grad|=6.912107e-02 g(8,9)=3.297007e-01
    [Grad] min=-8.804390e-04 max=2.585786e+00 avg|grad|=6.971560e-02 g(8,9)=3.324479e-01
    [Grad] min=-9.151262e-04 max=2.608788e+00 avg|grad|=7.030861e-02 g(8,9)=3.351835e-01
    [Grad] min=-9.487366e-04 max=2.631744e+00 avg|grad|=7.089926e-02 g(8,9)=3.379037e-01
    [Grad] min=-9.812626e-04 max=2.654626e+00 avg|grad|=7.148690e-02 g(8,9)=3.406046e-01
    [Grad] min=-1.012687e-03 max=2.677405e+00 avg|grad|=7.207059e-02 g(8,9)=3.432819e-01
    [Grad] min=-1.042976e-03 max=2.700049e+00 avg|grad|=7.264932e-02 g(8,9)=3.459312e-01
Iter  150  Loss: 5.299067e+00  α ∈ [0.200000, 0.800000]  Left(8,9)=0.200000
    [Grad] min=-1.072089e-03 max=2.722521e+00 avg|grad|=7.322269e-02 g(8,9)=3.485475e-01
    [Grad] min=-1.099981e-03 max=2.744784e+00 avg|grad|=7.378952e-02 g(8,9)=3.511256e-01
    [Grad] min=-1.126610e-03 max=2.766796e+00 avg|grad|=7.434818e-02 g(8,9)=3.536599e-01
    [Grad] min=-1.151930e-03 max=2.788510e+00 avg|grad|=7.489780e-02 g(8,9)=3.561441e-01
    [Grad] min=-1.175909e-03 max=2.809878e+00 avg|grad|=7.543669e-02 g(8,9)=3.585716e-01
    [Grad] min=-1.198506e-03 max=2.830845e+00 avg|grad|=7.596328e-02 g(8,9)=3.609351e-01
    [Grad] min=-1.219691e-03 max=2.851352e+00 avg|grad|=7.647595e-02 g(8,9)=3.632267e-01
    [Grad] min=-1.239428e-03 max=2.871334e+00 avg|grad|=7.697295e-02 g(8,9)=3.654380e-01
    [Grad] min=-1.257680e-03 max=2.890724e+00 avg|grad|=7.745255e-02 g(8,9)=3.675597e-01
    [Grad] min=-1.274448e-03 max=2.909444e+00 avg|grad|=7.791307e-02 g(8,9)=3.695818e-01
Iter  160  Loss: 4.098432e+00  α ∈ [0.180000, 0.820000]  Left(8,9)=0.180000
    [Grad] min=-1.289738e-03 max=2.927413e+00 avg|grad|=7.835171e-02 g(8,9)=3.714935e-01
    [Grad] min=-1.303479e-03 max=2.944542e+00 avg|grad|=7.876604e-02 g(8,9)=3.732833e-01
    [Grad] min=-1.323923e-03 max=2.960732e+00 avg|grad|=7.915392e-02 g(8,9)=3.749385e-01
    [Grad] min=-1.395189e-03 max=2.975879e+00 avg|grad|=7.951246e-02 g(8,9)=3.764456e-01
    [Grad] min=-1.461229e-03 max=2.989866e+00 avg|grad|=7.983812e-02 g(8,9)=3.777898e-01
    [Grad] min=-1.521969e-03 max=3.002567e+00 avg|grad|=8.012753e-02 g(8,9)=3.789553e-01
    [Grad] min=-1.577331e-03 max=3.013847e+00 avg|grad|=8.037706e-02 g(8,9)=3.799249e-01
    [Grad] min=-1.627028e-03 max=3.023553e+00 avg|grad|=8.058358e-02 g(8,9)=3.806802e-01
    [Grad] min=-1.671031e-03 max=3.031524e+00 avg|grad|=8.074194e-02 g(8,9)=3.812011e-01
    [Grad] min=-1.709281e-03 max=3.037583e+00 avg|grad|=8.084746e-02 g(8,9)=3.814658e-01
Iter  170  Loss: 2.835397e+00  α ∈ [0.160000, 0.839907]  Left(8,9)=0.160000
    [Grad] min=-1.741644e-03 max=3.041534e+00 avg|grad|=8.089507e-02 g(8,9)=3.814506e-01
    [Grad] min=-1.767972e-03 max=3.043169e+00 avg|grad|=8.087924e-02 g(8,9)=3.811299e-01
    [Grad] min=-1.788102e-03 max=3.042256e+00 avg|grad|=8.079412e-02 g(8,9)=3.804759e-01
    [Grad] min=-1.801963e-03 max=3.038546e+00 avg|grad|=8.063376e-02 g(8,9)=3.794588e-01
    [Grad] min=-1.809271e-03 max=3.031766e+00 avg|grad|=8.039095e-02 g(8,9)=3.780462e-01
    [Grad] min=-1.809709e-03 max=3.021621e+00 avg|grad|=8.005727e-02 g(8,9)=3.762035e-01
    [Grad] min=-1.802928e-03 max=3.007794e+00 avg|grad|=7.962437e-02 g(8,9)=3.738936e-01
    [Grad] min=-1.788548e-03 max=2.989942e+00 avg|grad|=7.908319e-02 g(8,9)=3.710772e-01
    [Grad] min=-1.797292e-03 max=2.967746e+00 avg|grad|=7.842433e-02 g(8,9)=3.676591e-01
    [Grad] min=-1.797356e-03 max=2.941028e+00 avg|grad|=7.764021e-02 g(8,9)=3.636227e-01
Iter  180  Loss: 1.582226e+00  α ∈ [0.140024, 0.858620]  Left(8,9)=0.140083
    [Grad] min=-1.783854e-03 max=2.909313e+00 avg|grad|=7.672429e-02 g(8,9)=3.589667e-01
    [Grad] min=-1.756600e-03 max=2.872265e+00 avg|grad|=7.566838e-02 g(8,9)=3.536557e-01
    [Grad] min=-1.715444e-03 max=2.829547e+00 avg|grad|=7.446439e-02 g(8,9)=3.476546e-01
    [Grad] min=-1.660328e-03 max=2.780827e+00 avg|grad|=7.310445e-02 g(8,9)=3.409304e-01
    [Grad] min=-1.591212e-03 max=2.725786e+00 avg|grad|=7.158118e-02 g(8,9)=3.334519e-01
    [Grad] min=-1.509064e-03 max=2.664122e+00 avg|grad|=6.988804e-02 g(8,9)=3.251917e-01
    [Grad] min=-1.414180e-03 max=2.595569e+00 avg|grad|=6.801856e-02 g(8,9)=3.161268e-01
    [Grad] min=-1.306601e-03 max=2.519899e+00 avg|grad|=6.596796e-02 g(8,9)=3.062405e-01
    [Grad] min=-1.186444e-03 max=2.436943e+00 avg|grad|=6.373306e-02 g(8,9)=2.955236e-01
    [Grad] min=-1.053911e-03 max=2.346608e+00 avg|grad|=6.131267e-02 g(8,9)=2.839767e-01
Iter  190  Loss: 5.761253e-01  α ∈ [0.121513, 0.874001]  Left(8,9)=0.121867
    [Grad] min=-9.093110e-04 max=2.248894e+00 avg|grad|=5.870809e-02 g(8,9)=2.716120e-01
    [Grad] min=-7.535960e-04 max=2.143946e+00 avg|grad|=5.592413e-02 g(8,9)=2.584569e-01
    [Grad] min=-6.136121e-04 max=2.032073e+00 avg|grad|=5.296959e-02 g(8,9)=2.445562e-01
    [Grad] min=-4.751497e-04 max=1.913652e+00 avg|grad|=4.985589e-02 g(8,9)=2.299698e-01
    [Grad] min=-3.773983e-04 max=1.789300e+00 avg|grad|=4.660069e-02 g(8,9)=2.147801e-01
    [Grad] min=-2.823574e-04 max=1.659844e+00 avg|grad|=4.322558e-02 g(8,9)=1.990923e-01
    [Grad] min=-2.135611e-04 max=1.526333e+00 avg|grad|=3.975927e-02 g(8,9)=1.830349e-01
    [Grad] min=-1.520523e-04 max=1.390045e+00 avg|grad|=3.623675e-02 g(8,9)=1.667597e-01
    [Grad] min=-9.341189e-05 max=1.252478e+00 avg|grad|=3.269533e-02 g(8,9)=1.504404e-01
    [Grad] min=-3.693033e-05 max=1.115324e+00 avg|grad|=2.917809e-02 g(8,9)=1.342689e-01
Iter  200  Loss: 9.208302e-02  α ∈ [0.107555, 0.881366]  Left(8,9)=0.108187
  -> alpha_inferred.csv written

Inversion finished. Compare alpha_true.csv and alpha_inferred.csv.
If left half shows low conductivity (≈0.1) and right half high (≈0.8), the DeepFDM logic succeeded.

```

---

## 0.3 表格

### 0.3.1 alpha_true.csv

```csv
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000
0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.10000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000,0.80000000

```

### 0.3.2 temp_target.csv

```csv
0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000
0.00000000,0.17896941,0.36042165,0.54643321,0.73820775,0.93550450,1.13595941,1.33438543,1.52229059,1.68804004,1.81817046,1.90007480,1.92546873,1.89315700,1.80957861,1.68680818,1.53917859,1.38017326,1.22057204,1.06789937,0.92671991,0.79904428,0.68555980,0.58592721,0.49921677,0.42420697,0.35957567,0.30401448,0.25629145,0.21528092,0.17997306,0.14947176,0.12298609,0.09981862,0.07935256,0.06103875,0.04438303,0.02893426,0.01427285,0.00000000
0.00000000,0.35551568,0.71640239,1.08727877,1.47112269,1.86813045,2.27427184,2.67965089,3.06711496,3.41207390,3.68491954,3.85699007,3.90897350,3.83793923,3.65874546,3.39889578,3.09015397,2.76134443,2.43458572,2.12463583,1.83997160,1.58392807,1.35729412,1.15895499,0.98675244,0.83805222,0.71009565,0.60020311,0.50588092,0.42486811,0.35514713,0.29493427,0.24265928,0.19694017,0.15655634,0.12042209,0.08756110,0.05708244,0.02815780,0.00000000
0.00000000,0.52680866,1.06262707,1.61550259,2.19132591,2.79217147,3.41397117,4.04348139,4.65501401,5.20855130,5.65240953,5.93364325,6.01509339,5.89073394,5.58881768,5.16044975,4.66193389,4.14123128,3.63251507,3.15673729,2.72467280,2.33946278,2.00078569,1.70589102,1.45082434,1.23118691,1.04257999,0.88084569,0.74218177,0.62318112,0.52082802,0.43247145,0.35578704,0.28873498,0.22951741,0.17653748,0.12836074,0.08367913,0.04127716,0.00000000
0.00000000,0.68926447,1.39213703,2.12128513,2.88716656,3.69604227,4.54677138,5.42583395,6.30051064,7.11233230,7.77737402,8.20257590,8.31906287,8.11489840,7.64198748,6.99121218,6.25632491,5.51006973,4.79852756,4.14607246,3.56262230,3.04855320,2.60057118,2.21306476,1.87952346,1.59333963,1.34823334,1.13845350,0.95884979,0.80487244,0.67253429,0.55835486,0.45929777,0.37270785,0.29625090,0.22785746,0.16567096,0.10799993,0.05327355,0.00000000
0.00000000,0.83833489,1.69581318,2.59098622,3.54084873,4.55896856,5.65176059,6.81121537,8.00213334,9.14540757,10.10981417,10.73527848,10.89624623,10.56971108,9.84993542,8.89589195,7.85932485,6.84446995,5.90656778,5.06759075,4.33132286,3.69167090,3.13997961,2.66635815,2.26093804,1.91447745,1.61861393,1.36593119,1.14993070,0.96495795,0.80611011,0.66913966,0.55036106,0.44656395,0.35493387,0.27298056,0.19847308,0.12938087,0.06381951,0.00000000
0.00000000,0.96852861,1.96232309,3.00677203,4.12721745,5.34793078,6.68876937,8.15587867,9.72031538,11.28299017,12.64716448,13.54802688,13.76644232,13.26058078,12.19501459,10.83761153,9.42505953,8.09878038,6.91626409,5.88769690,5.00356367,4.24696472,3.60143620,3.05155175,2.58348043,2.18509322,1.84587810,1.55678169,1.31003091,1.09895855,0.91784240,0.76176104,0.62646652,0.50827241,0.40395563,0.31066987,0.22586873,0.14723675,0.07262647,0.00000000
0.00000000,1.07375985,2.17877833,3.34742440,4.61423759,6.01654585,7.59276044,9.37208346,11.34272140,13.39023346,15.23808875,16.48066876,16.76789231,16.03110840,14.53378980,12.69288647,10.85375240,9.19639929,7.77063662,6.56450436,5.54844250,4.69134319,3.96738295,3.35504779,2.83643808,2.39662224,2.02309678,1.70534923,1.43450620,1.20304846,1.00457840,0.83362796,0.68549848,0.55612560,0.44196407,0.33988827,0.24710526,0.16107750,0.07945286,0.00000000
0.00000000,1.14806413,2.33225902,3.59081925,4.96647045,6.50903354,8.27616033,10.32326160,12.66187747,15.16915459,17.49089069,19.07436272,19.42904935,18.45722458,16.52884296,14.21855465,11.98146216,10.03026801,8.39995528,7.05182776,5.93453668,5.00275119,4.22185139,3.56494477,3.01071058,2.54195426,2.14461762,1.80708082,1.51965471,1.27420049,1.06383692,0.88270932,0.72580328,0.58879159,0.46790612,0.35982887,0.26159753,0.17052231,0.08411102,0.00000000
0.00000000,1.18658845,2.41206167,3.71804113,5.15215286,6.77203032,8.64792455,10.85325696,13.41689965,16.21317564,18.83850758,20.64267730,21.04071428,19.91522856,17.70794871,15.09878654,12.61418828,10.48575122,8.73624365,7.30808175,6.13529991,5.16345063,4.35248188,3.67230313,3.09962029,2.61596545,2.20642341,1.85877430,1.56289329,1.31031456,1.09390407,0.90760659,0.74624491,0.60535690,0.48106046,0.36993943,0.26894527,0.17531081,0.08647265,0.00000000
0.00000000,1.18658845,2.41206167,3.71804113,5.15215286,6.77203032,8.64792455,10.85325696,13.41689965,16.21317564,18.83850758,20.64267730,21.04071428,19.91522856,17.70794871,15.09878654,12.61418828,10.48575122,8.73624365,7.30808175,6.13529991,5.16345063,4.35248188,3.67230313,3.09962029,2.61596545,2.20642341,1.85877430,1.56289329,1.31031456,1.09390407,0.90760659,0.74624491,0.60535690,0.48106046,0.36993943,0.26894527,0.17531081,0.08647265,0.00000000
0.00000000,1.14806413,2.33225902,3.59081925,4.96647045,6.50903354,8.27616033,10.32326160,12.66187747,15.16915459,17.49089069,19.07436272,19.42904935,18.45722458,16.52884296,14.21855465,11.98146216,10.03026801,8.39995528,7.05182776,5.93453668,5.00275119,4.22185139,3.56494477,3.01071058,2.54195426,2.14461762,1.80708082,1.51965471,1.27420049,1.06383692,0.88270932,0.72580328,0.58879159,0.46790612,0.35982887,0.26159753,0.17052231,0.08411102,0.00000000
0.00000000,1.07375985,2.17877833,3.34742440,4.61423759,6.01654585,7.59276044,9.37208346,11.34272140,13.39023346,15.23808875,16.48066876,16.76789231,16.03110840,14.53378980,12.69288647,10.85375240,9.19639929,7.77063662,6.56450436,5.54844250,4.69134319,3.96738295,3.35504779,2.83643808,2.39662224,2.02309678,1.70534923,1.43450620,1.20304846,1.00457840,0.83362796,0.68549848,0.55612560,0.44196407,0.33988827,0.24710526,0.16107750,0.07945286,0.00000000
0.00000000,0.96852861,1.96232309,3.00677203,4.12721745,5.34793078,6.68876937,8.15587867,9.72031538,11.28299017,12.64716448,13.54802688,13.76644232,13.26058078,12.19501459,10.83761153,9.42505953,8.09878038,6.91626409,5.88769690,5.00356367,4.24696472,3.60143620,3.05155175,2.58348043,2.18509322,1.84587810,1.55678169,1.31003091,1.09895855,0.91784240,0.76176104,0.62646652,0.50827241,0.40395563,0.31066987,0.22586873,0.14723675,0.07262647,0.00000000
0.00000000,0.83833489,1.69581318,2.59098622,3.54084873,4.55896856,5.65176059,6.81121537,8.00213334,9.14540757,10.10981417,10.73527848,10.89624623,10.56971108,9.84993542,8.89589195,7.85932485,6.84446995,5.90656778,5.06759075,4.33132286,3.69167090,3.13997961,2.66635815,2.26093804,1.91447745,1.61861393,1.36593119,1.14993070,0.96495795,0.80611011,0.66913966,0.55036106,0.44656395,0.35493387,0.27298056,0.19847308,0.12938087,0.06381951,0.00000000
0.00000000,0.68926447,1.39213703,2.12128513,2.88716656,3.69604227,4.54677138,5.42583395,6.30051064,7.11233230,7.77737402,8.20257590,8.31906287,8.11489840,7.64198748,6.99121218,6.25632491,5.51006973,4.79852756,4.14607246,3.56262230,3.04855320,2.60057118,2.21306476,1.87952346,1.59333963,1.34823334,1.13845350,0.95884979,0.80487244,0.67253429,0.55835486,0.45929777,0.37270785,0.29625090,0.22785746,0.16567096,0.10799993,0.05327355,0.00000000
0.00000000,0.52680866,1.06262707,1.61550259,2.19132591,2.79217147,3.41397117,4.04348139,4.65501401,5.20855130,5.65240953,5.93364325,6.01509339,5.89073394,5.58881768,5.16044975,4.66193389,4.14123128,3.63251507,3.15673729,2.72467280,2.33946278,2.00078569,1.70589102,1.45082434,1.23118691,1.04257999,0.88084569,0.74218177,0.62318112,0.52082802,0.43247145,0.35578704,0.28873498,0.22951741,0.17653748,0.12836074,0.08367913,0.04127716,0.00000000
0.00000000,0.35551568,0.71640239,1.08727877,1.47112269,1.86813045,2.27427184,2.67965089,3.06711496,3.41207390,3.68491954,3.85699007,3.90897350,3.83793923,3.65874546,3.39889578,3.09015397,2.76134443,2.43458572,2.12463583,1.83997160,1.58392807,1.35729412,1.15895499,0.98675244,0.83805222,0.71009565,0.60020311,0.50588092,0.42486811,0.35514713,0.29493427,0.24265928,0.19694017,0.15655634,0.12042209,0.08756110,0.05708244,0.02815780,0.00000000
0.00000000,0.17896941,0.36042165,0.54643321,0.73820775,0.93550450,1.13595941,1.33438543,1.52229059,1.68804004,1.81817046,1.90007480,1.92546873,1.89315700,1.80957861,1.68680818,1.53917859,1.38017326,1.22057204,1.06789937,0.92671991,0.79904428,0.68555980,0.58592721,0.49921677,0.42420697,0.35957567,0.30401448,0.25629145,0.21528092,0.17997306,0.14947176,0.12298609,0.09981862,0.07935256,0.06103875,0.04438303,0.02893426,0.01427285,0.00000000
0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000,0.00000000

```

### 0.3.3 alpha_inferred.csv

```csv
0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000
0.50000000,0.57666422,0.67404685,0.74306043,0.79005477,0.82147833,0.84248848,0.85675347,0.86664632,0.87375402,0.87813001,0.88029694,0.88066922,0.87963341,0.87706191,0.87284098,0.86680583,0.85934566,0.85021847,0.83932672,0.82663342,0.81195775,0.79524048,0.77655108,0.75614614,0.73458574,0.71226667,0.68951381,0.66663971,0.64394537,0.62172815,0.60029138,0.57995304,0.56105291,0.54395891,0.52907122,0.51681901,0.50764063,0.50193715,0.50000000
0.50000000,0.67148391,0.78357181,0.83376943,0.85799940,0.87131986,0.87737008,0.87962415,0.88136616,0.83990894,0.72636117,0.66330593,0.65393555,0.69913640,0.79775416,0.88100495,0.87917562,0.87708133,0.87475834,0.87193411,0.86774588,0.86154589,0.85399114,0.84512208,0.83469482,0.82245682,0.80818479,0.79167889,0.77280037,0.75148313,0.72796992,0.70244463,0.67500722,0.64586097,0.61538693,0.58430197,0.55397373,0.52693801,0.50732844,0.50000000
0.50000000,0.73613175,0.83083415,0.86178805,0.87366125,0.87708304,0.87936919,0.87997559,0.65523263,0.48327931,0.37545691,0.32137548,0.31326177,0.35027383,0.43853166,0.59045434,0.80704628,0.87855038,0.87627607,0.87398800,0.87175725,0.86954246,0.86674700,0.86242152,0.85615265,0.84877638,0.84000735,0.82951362,0.81690575,0.80175098,0.78356042,0.76182193,0.73616590,0.70660053,0.67287739,0.63494025,0.59344038,0.55100697,0.51511307,0.50000000
0.50000000,0.77831929,0.85276368,0.87208559,0.87616544,0.87858967,0.83917087,0.55454386,0.34352464,0.20041363,0.12647178,0.11733764,0.11638323,0.12064423,0.15289862,0.28742486,0.47448128,0.74999496,0.87745804,0.87509547,0.87286439,0.87069681,0.86858504,0.86651483,0.86369952,0.85900996,0.85285459,0.84549585,0.83655677,0.82556049,0.81190987,0.79483262,0.77343071,0.74665328,0.71384376,0.67433387,0.62753409,0.57470382,0.52400967,0.50000000
0.50000000,0.80553193,0.86433422,0.87495923,0.87752694,0.87996977,0.55733058,0.29981038,0.12788250,0.11194609,0.10972855,0.10911365,0.10903589,0.10938378,0.11058006,0.11633276,0.22781804,0.46311076,0.80191318,0.87609367,0.87377298,0.87155684,0.86942461,0.86738542,0.86542784,0.86311130,0.85900767,0.85330405,0.84641197,0.83790049,0.82718346,0.81347157,0.79571003,0.77249831,0.74213270,0.70331027,0.65457001,0.59542048,0.53285855,0.50000000
0.50000000,0.82290929,0.87061468,0.87622500,0.87868105,0.66943236,0.34628323,0.12721551,0.11061026,0.10882938,0.10828646,0.10808950,0.10807627,0.10822142,0.10859524,0.10953854,0.11443471,0.26127566,0.56318962,0.87704341,0.87456144,0.87226863,0.87007860,0.86800565,0.86605566,0.86420122,0.86186819,0.85751738,0.85179764,0.84476169,0.83587320,0.82438021,0.80921159,0.78885603,0.76121988,0.72389872,0.67487084,0.61213341,0.54077033,0.50000000
0.50000000,0.83386205,0.87356468,0.87717207,0.87966798,0.49677290,0.20114351,0.11179190,0.10885482,0.10815058,0.10786420,0.10775523,0.10775767,0.10786148,0.10809701,0.10856181,0.10982865,0.12474527,0.39450651,0.79151908,0.87521070,0.87283516,0.87058599,0.86846569,0.86649028,0.86464878,0.86286281,0.85988491,0.85486463,0.84870484,0.84093283,0.83083977,0.81738282,0.79902773,0.77354202,0.73788195,0.68922208,0.62457582,0.54713211,0.50000000
0.50000000,0.84033952,0.87498618,0.87782003,0.77439008,0.39057725,0.12572484,0.10975251,0.10835800,0.10789633,0.10769223,0.10761229,0.10762050,0.10771050,0.10789733,0.10824291,0.10897790,0.11284078,0.29342255,0.66889721,0.87567871,0.87323372,0.87093754,0.86877780,0.86677570,0.86492640,0.86318633,0.86094079,0.85656051,0.85089632,0.84376853,0.83449560,0.82207055,0.80496870,0.78089995,0.74654207,0.69835595,0.63277836,0.55155531,0.50000000
0.50000000,0.84335601,0.87549169,0.87815300,0.71637449,0.34082693,0.11742619,0.10923309,0.10818664,0.10780116,0.10762389,0.10755518,0.10756572,0.10765007,0.10782168,0.10812765,0.10872740,0.11106202,0.24331970,0.60756874,0.87592726,0.87343893,0.87111800,0.86893683,0.86691797,0.86506129,0.86332844,0.86132953,0.85732153,0.85188282,0.84504676,0.83615615,0.82421836,0.80771246,0.78433523,0.75068107,0.70279129,0.63684388,0.55381710,0.50000000
0.50000000,0.84335601,0.87549169,0.87815300,0.71637449,0.34082693,0.11742619,0.10923309,0.10818664,0.10780116,0.10762389,0.10755518,0.10756572,0.10765007,0.10782168,0.10812765,0.10872740,0.11106202,0.24331970,0.60756874,0.87592726,0.87343893,0.87111800,0.86893683,0.86691797,0.86506129,0.86332844,0.86132953,0.85732153,0.85188282,0.84504676,0.83615615,0.82421836,0.80771246,0.78433523,0.75068107,0.70279129,0.63684388,0.55381710,0.50000000
0.50000000,0.84033952,0.87498618,0.87782003,0.77439008,0.39057725,0.12572484,0.10975251,0.10835800,0.10789633,0.10769223,0.10761229,0.10762050,0.10771050,0.10789733,0.10824291,0.10897790,0.11284078,0.29342255,0.66889721,0.87567871,0.87323372,0.87093754,0.86877780,0.86677570,0.86492640,0.86318633,0.86094079,0.85656051,0.85089632,0.84376853,0.83449560,0.82207055,0.80496870,0.78089995,0.74654207,0.69835595,0.63277836,0.55155531,0.50000000
0.50000000,0.83386205,0.87356468,0.87717207,0.87966798,0.49677290,0.20114351,0.11179190,0.10885482,0.10815058,0.10786420,0.10775523,0.10775767,0.10786148,0.10809701,0.10856181,0.10982865,0.12474527,0.39450651,0.79151908,0.87521070,0.87283516,0.87058599,0.86846569,0.86649028,0.86464878,0.86286281,0.85988491,0.85486463,0.84870484,0.84093283,0.83083977,0.81738282,0.79902773,0.77354202,0.73788195,0.68922208,0.62457582,0.54713211,0.50000000
0.50000000,0.82290929,0.87061468,0.87622500,0.87868105,0.66943236,0.34628323,0.12721551,0.11061026,0.10882938,0.10828646,0.10808950,0.10807627,0.10822142,0.10859524,0.10953854,0.11443471,0.26127566,0.56318962,0.87704341,0.87456144,0.87226863,0.87007860,0.86800565,0.86605566,0.86420122,0.86186819,0.85751738,0.85179764,0.84476169,0.83587320,0.82438021,0.80921159,0.78885603,0.76121988,0.72389872,0.67487084,0.61213341,0.54077033,0.50000000
0.50000000,0.80553193,0.86433422,0.87495923,0.87752694,0.87996977,0.55733058,0.29981038,0.12788250,0.11194609,0.10972855,0.10911365,0.10903589,0.10938378,0.11058006,0.11633276,0.22781804,0.46311076,0.80191318,0.87609367,0.87377298,0.87155684,0.86942461,0.86738542,0.86542784,0.86311130,0.85900767,0.85330405,0.84641197,0.83790050,0.82718346,0.81347157,0.79571003,0.77249831,0.74213270,0.70331027,0.65457001,0.59542047,0.53285855,0.50000000
0.50000000,0.77831929,0.85276368,0.87208559,0.87616544,0.87858967,0.83917087,0.55454386,0.34352464,0.20041363,0.12647178,0.11733764,0.11638323,0.12064423,0.15289862,0.28742486,0.47448128,0.74999496,0.87745804,0.87509547,0.87286439,0.87069681,0.86858504,0.86651483,0.86369952,0.85900996,0.85285459,0.84549585,0.83655677,0.82556049,0.81190987,0.79483262,0.77343071,0.74665328,0.71384376,0.67433387,0.62753409,0.57470383,0.52400967,0.50000000
0.50000000,0.73613175,0.83083415,0.86178805,0.87366125,0.87708304,0.87936919,0.87997560,0.65523263,0.48327931,0.37545691,0.32137548,0.31326177,0.35027383,0.43853166,0.59045434,0.80704628,0.87855038,0.87627607,0.87398800,0.87175725,0.86954246,0.86674700,0.86242152,0.85615265,0.84877638,0.84000735,0.82951362,0.81690575,0.80175098,0.78356042,0.76182193,0.73616590,0.70660053,0.67287739,0.63494025,0.59344038,0.55100697,0.51511308,0.50000000
0.50000000,0.67148391,0.78357181,0.83376943,0.85799940,0.87131986,0.87737008,0.87962415,0.88136616,0.83990894,0.72636117,0.66330593,0.65393555,0.69913640,0.79775416,0.88100495,0.87917562,0.87708133,0.87475834,0.87193411,0.86774588,0.86154589,0.85399114,0.84512208,0.83469482,0.82245682,0.80818479,0.79167889,0.77280037,0.75148313,0.72796993,0.70244463,0.67500722,0.64586097,0.61538693,0.58430197,0.55397373,0.52693800,0.50732844,0.50000000
0.50000000,0.57666422,0.67404685,0.74306043,0.79005477,0.82147833,0.84248848,0.85675347,0.86664632,0.87375402,0.87813001,0.88029694,0.88066922,0.87963341,0.87706191,0.87284098,0.86680583,0.85934566,0.85021847,0.83932672,0.82663342,0.81195775,0.79524048,0.77655108,0.75614614,0.73458574,0.71226667,0.68951382,0.66663971,0.64394537,0.62172815,0.60029138,0.57995303,0.56105291,0.54395891,0.52907122,0.51681902,0.50764063,0.50193715,0.50000000
0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000,0.50000000

```