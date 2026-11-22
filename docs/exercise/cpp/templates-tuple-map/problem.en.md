This task reads no stdin and writes no stdout. Implement a template that applies a function to every element of a tuple.

```cpp
#include <tuple>
#include <utility>

template <typename Tuple, typename F>
auto tuple_map(Tuple&& tuple, F&& f);
```

Rules:

- Return a `std::tuple` whose `i`-th element is `f(std::get<i>(tuple))`.
- Preserve element order and count; each result element's type is determined by `f`'s return type.
- Support the empty tuple: return an empty tuple and do not call `f`.
- Support `const` tuples without modifying the input.
- Support function pointers, lambdas, and function objects with overloaded `operator()`.
- Using `std::index_sequence`, `std::make_index_sequence`, and pack expansion is recommended.
- Do not write `main`, and do not change the signature.
