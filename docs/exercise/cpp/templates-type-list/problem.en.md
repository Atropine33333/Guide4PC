(The diviner brings out a deck whose faces are all type names, and not one card touches the table.)

Querent: This deck is never dealt into a hand, only laid out below?

Diviner: Exactly. `TypeList<Ts...>` exists only at compile time; the cards take not one inch of the world.

Querent: How is the count read?

Diviner: `size`, which is `sizeof...(Ts)`. An empty deck is 0.

Querent: And the first card?

Diviner: `front`. For an empty deck `front` is written as `void`, and no one turns that card.

Querent: And a new card pressed on top?

Diviner: `push_front<T>`, giving `TypeList<T, Ts...>`.

Querent: If I ask whether a card is in the deck?

Diviner: `contains<T>`: `true` if present, `false` if absent. A card that appears twice still counts once.

Querent: How is the card at `Index` taken?

Diviner: `at<Index>`, counted from 0; only positions the deck truly has.

Querent: Are these cards visible at run time?

Diviner: They are not. The faces are the types themselves, and the answer is settled at compile time.

**In plain words**: implement `TypeList<Ts...>`, which exists only at compile time, with `size`, `front`, `push_front`, `contains`, and `at`; every result must be available at compile time.

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
