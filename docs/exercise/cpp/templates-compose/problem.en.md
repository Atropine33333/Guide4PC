This task reads no stdin and writes no stdout. Implement a template that returns a composition of two callables.

```cpp
#include <utility>

template <typename F, typename G>
auto compose(F f, G g);
```

The returned callable `h` satisfies, for every single argument `x`:

```cpp
h(x) == f(g(x))
```

That is, call `g` first and pass its result to `f`.

Rules:

- Support function pointers, lambdas, and function objects with `const operator()`.
- The returned object stores `f` and `g` by value, can be called repeatedly, and can participate in nested compositions.
- `f` and `g` may have different result types; the compiler deduces `compose`'s return type.
- Do not return `std::function` (passing one in is allowed).
- Do not write `main`, and do not change the signature.
