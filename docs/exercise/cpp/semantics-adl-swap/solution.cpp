// Reference solution: semantics-adl-swap
#include <utility>

namespace gadget {

struct Widget {
    int id;
    int value;
};

namespace {
int g_calls = 0;
}

int swap_calls() {
    return g_calls;
}

void reset_swap_calls() {
    g_calls = 0;
}

void swap(Widget& a, Widget& b) {
    ++g_calls;
    std::swap(a.id, b.id);
    std::swap(a.value, b.value);
}

}  // namespace gadget
