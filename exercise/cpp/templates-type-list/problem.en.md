This task reads no stdin and writes no stdout. Implement a compile-time-only type list.

```cpp
#include <cstddef>

template <typename... Ts>
struct TypeList {
    static constexpr std::size_t size = sizeof...(Ts);

    using front = /* the first type */;
    template <typename T>
    using push_front = /* the new list */;

    template <typename T>
    static constexpr bool contains = /* whether T is present */;

    template <std::size_t Index>
    using at = /* the type at Index */;
};
```

Rules:

- `size` is the number of types; the empty list has size 0.
- `front` is the first type; define it as `void` for the empty list (tests will not read the empty front).
- `TypeList<Ts...>::push_front<T>` produces `TypeList<T, Ts...>`.
- `contains<T>` is a compile-time boolean; it is `false` for the empty list, and duplicates do not matter.
- `at<Index>` is the type at zero-based `Index`; tests only use valid indices.
- Every result must be available at compile time and must not rely on runtime containers.
- Do not write `main`, and do not change the public interface.
