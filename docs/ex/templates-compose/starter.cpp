#include <type_traits>
#include <utility>

template <typename F, typename G>
auto compose(F f, G g) {
    return [f, g](auto x) -> std::decay_t<decltype(f(g(x)))> {
        // TODO: return f(g(x)); a default value is not a real composition.
        (void)f;
        (void)g;
        (void)x;
        return {};
    };
}
