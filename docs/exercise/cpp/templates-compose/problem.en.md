Huizi said to Zhuangzi: "I have two vessels. The lower takes a thing, changes its form, and gives it out; the upper takes that form, changes it again, and gives it out. May I join them into one?"

Zhuangzi said: "You may. Cast a thing into the lower, take its result, cast that into the upper. The result you hold is the result of the two in succession."

Huizi said: "May vessels be stacked?"

Zhuangzi said: "Within a vessel there may be another. Stack without limit: above the upper there is an upper, below the lower there is a lower, each following each, and no end is reached."

Huizi said: "May the two results differ in form?"

Zhuangzi said: "They may. The lower gives out a circle, the upper takes the circle and gives out a square; that is allowed. Form follows the vessel; the way does not change."

Huizi said: "Where do the two vessels live inside the joined one?"

Zhuangzi said: "Hidden within, held by value. Take them and they serve; serve and they are taken again, not exhausted by one use."

**In plain words**: implement the function template `compose(F f, G g)`, which returns a callable `h` such that for every single argument `x`, `h(x) == f(g(x))` — call `g` first, pass its result to `f`.

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
