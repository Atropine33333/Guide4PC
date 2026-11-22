#include <tuple>
#include <type_traits>
#include <utility>

namespace starter_detail {

template <typename Tuple, typename F, std::size_t... Index>
auto tuple_map_impl(Tuple&& tuple, F&& f, std::index_sequence<Index...>) {
    // TODO: use f(std::get<Index>(tuple)) instead of default-constructed results.
    (void)tuple;
    (void)f;
    return std::make_tuple(
        std::decay_t<decltype(f(std::get<Index>(tuple)))>{}...);
}

}  // namespace starter_detail

template <typename Tuple, typename F>
auto tuple_map(Tuple&& tuple, F&& f) {
    return starter_detail::tuple_map_impl(
        std::forward<Tuple>(tuple), std::forward<F>(f),
        std::make_index_sequence<
            std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}
