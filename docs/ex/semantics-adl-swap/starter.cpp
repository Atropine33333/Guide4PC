namespace gadget {

struct Widget {
    int id;
    int value;
};

namespace {
int g_calls = 0;
}

int swap_calls() {
    // TODO: return the number of custom swaps performed.
    return 0;
}

void reset_swap_calls() {
    // TODO: reset the counter.
}

void swap(Widget& a, Widget& b) {
    (void)a;
    (void)b;
    // TODO: swap both members and increment the counter.
}

}  // namespace gadget
