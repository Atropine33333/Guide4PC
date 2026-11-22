(Late night, a shopping channel. The tubes hum; the host raises a calculator.)

Host: Folks, look at this `sum_all`! Pass it one `char` — it adds! Pass it two `int`s — it adds! Pass it five `long long`s — it still adds!

Announcer: BUT WAIT, THERE'S MORE!

Host: That's right, there's more! No limit on the parameter pack — one is not too few, ten is not too many!

Host: Empty? Empty returns 0! Every argument is added as a 64-bit signed integer; a `char` counts by its value — `'a'` is 97!

Host: Negative result? No problem! Just don't keep the total in an `int`, and it holds steady!

Announcer: And that's not all! It doesn't even want a `main`!

Host: Right — no `main`, no money down, just one fold expression!

(Cut to commercial.)

**In plain words**: implement the variadic function template `sum_all`, which adds every argument as a 64-bit signed integer and returns 0 for an empty pack; a `char` contributes its numeric value.

```cpp
template <typename... Args>
long long sum_all(Args... args);
```

Rules:

- Add all arguments as 64-bit signed integers and return the total.
- Return 0 when called with no arguments.
- Arguments may be `char`, `short`, `int`, `long`, `long long`, and so on, possibly mixed.
- A `char` contributes its numeric value (for example `'a'` is 97), not text.
- The result may be negative; do not accumulate in `int`.
- Do not write `main`.

### Hint

A C++17 fold expression is the easiest approach: `return (0LL + ... + static_cast<long long>(args));`. With an empty pack this fold produces 0.
