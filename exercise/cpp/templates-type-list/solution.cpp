// Reference solution: templates-type-list
#include <cstddef>
#include <tuple>
#include <type_traits>

template <typename... Ts>
struct TypeList;

namespace solution_detail {

template <typename List>
struct Front;

template <typename First, typename... Rest>
struct Front<TypeList<First, Rest...>> {
    using type = First;
};

template <>
struct Front<TypeList<>> {
    using type = void;
};

}  // namespace solution_detail

template <typename... Ts>
struct TypeList {
    static constexpr std::size_t size = sizeof...(Ts);

    using front = typename solution_detail::Front<TypeList>::type;

    template <typename T>
    using push_front = TypeList<T, Ts...>;

    template <typename T>
    static constexpr bool contains = (std::is_same_v<T, Ts> || ...);

    template <std::size_t Index>
    using at = std::tuple_element_t<Index, std::tuple<Ts...>>;
};
