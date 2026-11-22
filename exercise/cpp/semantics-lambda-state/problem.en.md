Use lambdas to implement two closure factories.

```cpp
std::function<int()> make_counter(int start);
std::function<int(int)> make_accumulator(int start);
```

Rules:

- `make_counter(start)` returns a counter: each call increments the internal count and returns the new value; the first call returns `start + 1`.
- `make_accumulator(start)` returns an accumulator: each call adds its argument to the internal total and returns the new total; the first call returns `start + argument`.
- The closure state is captured by value, so two closures never share state.
- Copying a `std::function` copies the closure's current state; afterwards the original and the copy advance independently.
- `start` and the arguments may be negative or zero.
- Do not write `main`.

### Hint

Use an init-capture to hold the state and mark the lambda `mutable`; otherwise a by-value capture cannot be modified.
