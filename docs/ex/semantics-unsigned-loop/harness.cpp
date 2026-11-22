// Guide4PC harness appended to the student's code in the same translation unit.
#include <climits>
#include <iostream>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

}  // namespace

int main() {
    report("less-safe", unsigned_less(5, 10) && !unsigned_less(10, 5) &&
                            !unsigned_less(0, 0) && unsigned_less(-1, 0) &&
                            unsigned_less(-100, 5));

    report("less-huge", unsigned_less(INT_MAX, 2147483648u) &&
                            unsigned_less(-1, 4294967295u) &&
                            !unsigned_less(INT_MAX, 2147483647u) &&
                            unsigned_less(0, 4294967295u));

    report("wrap-simple", wrap_decrement(5, 3) == 2 && wrap_decrement(0, 0) == 0 &&
                              wrap_decrement(1, 0) == 1 &&
                              wrap_decrement(4294967295u, 4294967295u) == 0);

    report("wrap-underflow", wrap_decrement(0, 1) == 4294967295u &&
                                 wrap_decrement(2, 5) == 4294967293u &&
                                 wrap_decrement(0, 4294967295u) == 1);

    report("wrap-chain",
           wrap_decrement(wrap_decrement(7, 4), 5) == 4294967294u);

    report("countdown-small", countdown_loops(0) == 0 && countdown_loops(1) == 1 &&
                                  countdown_loops(5) == 5 && countdown_loops(100) == 100);

    report("countdown-max", countdown_loops(static_cast<unsigned>(INT_MAX)) == INT_MAX);

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
