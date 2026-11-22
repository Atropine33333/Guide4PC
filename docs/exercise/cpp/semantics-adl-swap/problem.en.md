This task reads no stdin and writes no stdout. Implement a custom `swap` inside namespace `gadget` and observe how argument-dependent lookup (ADL) affects unqualified calls.

```cpp
namespace gadget {

struct Widget {
    int id;
    int value;
};

int swap_calls();
void reset_swap_calls();

// Custom swap: exchange two Widgets and increment swap_calls() by one.
void swap(Widget& a, Widget& b);

}  // namespace gadget
```

Rules:

- `gadget::swap(Widget&, Widget&)` swaps the `id` and `value` members and increments the counter returned by `swap_calls()` once per call.
- `reset_swap_calls()` resets the counter to zero.
- The counter is global state inside namespace `gadget` and starts at 0.
- Do not write `main`. The harness uses three call forms:
  - Inside a function it writes `using std::swap; swap(x, y);` where `x` and `y` are `gadget::Widget`. The argument types make ADL find `gadget::swap`, so the customization must be selected.
  - It writes `std::swap(x, y)`. This qualified call does not use ADL, so `gadget::swap` must not be called and the counter must not change; the Widgets are still exchanged through moves.
  - It writes `using std::swap; swap(a, b);` for two `int` values. `std::swap` must be selected and the `gadget` counter must stay unchanged.
- Do not use macros or specialize `std::swap`; the customization must be an ordinary function.
- Inside `gadget::swap`, swapping the `int` members with `std::swap` is fine and does not recurse.
