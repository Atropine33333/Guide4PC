#include <cstddef>

template <typename... Ts>
struct TypeList {
    static constexpr std::size_t size = sizeof...(Ts);

    using front = void;

    template <typename T>
    using push_front = TypeList<Ts...>;

    template <typename T>
    static constexpr bool contains = false;

    template <std::size_t Index>
    using at = void;
};
