// Reference solution: templates-tuple-map
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace solution_detail {

template <typename Tuple, typename F, std::size_t... Index>
auto tuple_map_impl(Tuple&& tuple, F&& f, std::index_sequence<Index...>) {
    return std::make_tuple(
        f(std::get<Index>(std::forward<Tuple>(tuple)))...);
}

}  // namespace solution_detail

template <typename Tuple, typename F>
auto tuple_map(Tuple&& tuple, F&& f) {
    return solution_detail::tuple_map_impl(
        std::forward<Tuple>(tuple), std::forward<F>(f),
        std::make_index_sequence<
            std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}
