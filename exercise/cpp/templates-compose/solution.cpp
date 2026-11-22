// Reference solution: templates-compose
#include <utility>

template <typename F, typename G>
auto compose(F f, G g) {
    return [f, g](auto x) {
        return f(g(x));
    };
}
