THE BOOK OF EXODUS, CHAPTER XIX

And the LORD spake unto Moses, saying: Speak unto all the congregation of the children of Israel, and say, When a name is called, seek it not only in its own house, but also among the tribe from which the argument came; and that which is found there is the law of that tribe. This law is called argument-dependent lookup.

And the LORD spake unto Moses again, saying: Set a `swap` among the people of `gadget`, and when two of them are exchanged, number one and write it in the book; and `reset_swap_calls` shall wipe the number from the book. He that calls `std::swap` by its full name passes by the law of the tribe; and the stranger `int` remains under the hand of `std::swap`.

And Moses set the `swap` among `gadget`, and numbered the people, as the LORD had commanded.

**In plain words**: Inside namespace `gadget`, implement a custom `swap(Widget&, Widget&)` that exchanges the `id` and `value` members and increments `swap_calls()` once per call; also implement `swap_calls()` and `reset_swap_calls()`, with the counter starting at 0. Three call forms must hold: the unqualified call must pick the customization, `std::swap` must bypass it, and two `int` values must stay with `std::swap`.

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

### Rules

- `gadget::swap(Widget&, Widget&)` swaps the `id` and `value` members and increments the counter returned by `swap_calls()` once per call.
- `reset_swap_calls()` resets the counter to zero.
- The counter is global state inside namespace `gadget` and starts at 0.
- Do not write `main`.
- Three call forms must hold:
  - Inside a function, write `using std::swap; swap(x, y);` where `x` and `y` are `gadget::Widget`. The argument types make ADL find `gadget::swap`, so the customization must be selected.
  - A qualified call `std::swap(x, y)` does not use ADL, so `gadget::swap` must not be called and the counter must not change; the Widgets are still exchanged through moves.
  - For two `int` values, write `using std::swap; swap(a, b);`; `std::swap` must be selected and the `gadget` counter must stay unchanged.
- Do not use macros or specialize `std::swap`; the customization must be an ordinary function.
- Inside `gadget::swap`, swapping the `int` members with `std::swap` is fine and does not recurse.
