# The Bumpy Road to DeepFDM – A Personal Chronicle

I'm the author of this guide. To teach C++ syntax and show how to bridge theory with engineering practice, I designed a 2D heat‑conduction inversion experiment based on the DeepFDM paper. Little did I know that this seemingly simple example would put me through a week of failed runs, frustration, and sudden epiphanies – and finally, it worked. Not perfectly, but it's precisely that imperfection that made the journey so meaningful. I want to write down this mental odyssey – partly as a reflection for myself, and partly in the hope that it might offer some comfort to others struggling with scientific computing.

## I. Initial Ambition – Recovering the Diffusivity Field with Finite‑Difference Perturbation

My task was straightforward: given a rectangular heat sink with an unknown thermal diffusivity field α(x,y), but with access to the final temperature field after a certain time, use the finite‑difference perturbation method – no automatic differentiation, no third‑party frameworks – to estimate the gradient by numerical perturbations, and then recover the entire α field via gradient descent.

I built the code following the classic textbook framework:
- A fixed 3×3 Laplacian kernel, upwind advection, and explicit Euler time marching.
- Perturb each grid point α[i][j] by a tiny ε = 1e-5, run a full forward simulation, and approximate the gradient with central differences.
- Use MSE as the loss function and iterate to update α.

The ground‑truth α field was set to high conductivity on the left (copper, α=0.8), with a vertical air gap (α=0.1) on the right. The heat source was placed on the left (x=0.3, y=0.25) to mimic chip heating. I hit Enter with high hopes, expecting the inversion to magically "discover" that air gap.

## II. The First Blow – The α Field Refused to Budge

The first run left me stunned: the loss did decrease, but the α field barely moved from the initial guess of 0.5 – its maximum crept up slightly, and its minimum barely twitched. In other words, the algorithm only learned to "raise the overall conductivity" to lower the loss, completely failing to recover the low‑conductivity channel on the right.

I immediately tried the usual tuning tricks:
- Increase the perturbation step ε to 0.01 to make gradients more visible.
- Raise the learning rate to 5.0, then even to 200 – at which point α slammed straight into the boundary at 1.0.
- Introduce RMSprop for adaptive learning rates to balance the update magnitudes across regions.
- Add spatial smoothing regularisation to force adjacent pixels to change together.

None of them worked. Gradient diagnostics revealed that at a point inside the air gap (12,9), the raw gradient was negative – meaning **increasing α at that point would lower the loss**. In other words, the algorithm thought that filling the air gap with copper would make the predicted temperature field match the target better!

## III. Lost in the Dark – A Tug‑of‑War with the Gradient Sign

I added more and more print statements, plotted 1D slices of the loss surface, and repeatedly checked the forward solver. Was the advection term misbehaving? Set to zero. Source amplitude too weak? Bumped to 500. Simulation steps too few? Extended to 8000. I even widened the air gap to cover the entire right half – still no luck. The strangest part: when I moved the air gap to the left and the heat source there too, the gradient suddenly turned positive – lowering α finally reduced the loss.

That was the moment it hit me: **the problem wasn't the optimizer – it was the physical identifiability of the problem itself**.

The essence of inversion is: can the observed temperature field uniquely determine the parameters? In my original setup, the heat source was on the left and the air gap on the right. The heat generated on the left diffused to the right, but the air gap's obstruction to the heat flow was extremely weak – because it was far downstream from the source, with very little temperature difference across it. In other words, changing the air gap's diffusivity had a much smaller effect on the temperature field than changing the background diffusivity near the heat source. Therefore, the global minimum of the loss function was not the true parameter field, but a configuration where "everything has high conductivity". No matter how I optimised, gradient descent faithfully marched towards that wrong minimum.

This is the famous "ill‑posedness" of inverse problems. Although the soul of DeepFDM is to embed physics through fixed numerical stencils, if the observational data itself is insensitive to the parameters, even a deity couldn't save it.

## IV. Light at the End of the Tunnel – Moving the Low‑Conductivity Zone to the Same Side as the Source

After realising this, I decisively changed the ground‑truth configuration: place the air gap on the **left** (x<0.5), keep the heat source inside it, and make the right side pure copper. Now the air gap acted like a blanket wrapped around the heat source – heat could hardly escape, so the temperature on the left rose significantly. With the initial guess α=0.5, heat would dissipate too quickly, leaving the left side too cold. To match the target high temperatures, the algorithm had to lower α there. This time the gradient was positive, and gradient descent naturally drove α downward.

To further boost sensitivity, I also adopted a multi‑time observation loss – recording temperature snapshots every 500 steps instead of only the final steady state. This amplified the air gap's blocking effect during early diffusion, giving the inversion a much stronger signal.

I ran it again, and a miracle happened: after just 20 iterations, the left‑side α dropped from 0.5 to 0.158; by iteration 30, it was already approaching 0.103, while the right side rose to about 0.8. The loss plummeted, and the 2D contour plot clearly showed a sharp vertical line – low on the left, high on the right. I let out a long breath – *this* was the DeepFDM I had been hoping for.

## V. Polishing the Edges – From Oscillation to Smooth Convergence

However, after nearing the true values, the gradients began to oscillate violently – positive and negative values alternating, occasionally spiking to astronomical numbers like 1e21. What was going on?

This is, in fact, the inherent numerical noise of the finite‑difference perturbation method. When the parameters are already close to the optimum, the loss surface becomes extremely flat, and the loss difference caused by perturbations drowns in round‑off errors – the gradient estimate becomes almost random. Coupled with RMSprop's accumulated history, which can lag behind, occasionally an anomalous update would slip through.

For the sake of a smooth teaching demonstration, I applied three tweaks:
- Reduced the learning rate from 0.01 to 0.002 to tame the step size.
- Added gradient clipping (element‑wise clamp to [-1,1]) after RMSprop normalisation, completely eliminating extreme gradients.
- Redirected all output to log files for easier post‑mortem analysis.

Finally, the results of 200 iterations were ready. I eagerly opened `alpha_inferred.csv` – and froze.

## VI. The Real Outcome – An "Insulating Bubble", Not a Perfect Global Recovery

The data told a clear story: the inferred diffusivity field was not a simple left‑low/right‑high step. Near the heat source (x≈0.3, y≈0.25), a low‑value region had indeed formed, bottoming out at 0.107 – like an insulating bubble wrapping around the source. Most of the right region had risen to around 0.85, close to the true copper value. However, the background region on the left, far from the source, stubbornly remained around 0.5 – nowhere near the expected 0.1. The boundary pixels barely moved from their initial 0.5 guess.

This was far from the "entire left half low‑conductivity" I had envisioned. At first I was disappointed, but then it struck me – this imperfect recovery is far more valuable than a perfect one.

Why did this happen? Because in the multi‑time loss, **changes in α near the heat source have a vastly larger impact on the temperature field than changes in the distant background**. The gradients concentrated around the source, and the optimiser naturally prioritised building an insulating shell right there to raise the temperature and match the target. As for the far‑left pixels, whether α was 0.1 or 0.8 made almost no difference to the temperature distribution – their gradients were nearly zero, so they remained almost untouched within a finite number of iterations. The algorithm had learned not the global distribution I had designed, but a **local effective model** – an insulating bubble around the heat source.

## VII. Reflection – Imperfection Is the Truest Teacher

If the algorithm had perfectly recovered my checkerboard pattern, that would have been the unreal part. It is precisely this flawed result that allowed me to ask a cascade of deeper questions:

- Why did the inversion only lower α near the heat source?
- How can we design the observation scheme – say, adding multiple heat sources, changing boundary conditions, or using full temporal data – to improve global identifiability?
- Could I introduce structural priors (like total variation regularisation) to encourage piecewise‑constant solutions and recover the sharp left‑right regions?

This project taught me profoundly that the soul of DeepFDM – or any physics‑informed inversion method – does not lie in fixed stencils or adaptive learning rates. It lies in **how much physical insight we actually inject into the loss function**, and whether we honestly confront the inherent ill‑posedness of inverse problems. An imperfect inversion often carries more weight than a perfectly tuned one – because it reveals the most genuine, fragile, and fascinating side of scientific computing.