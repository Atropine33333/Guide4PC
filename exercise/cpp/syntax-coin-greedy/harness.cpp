// Guide4PC harness appended to the student's code in the same translation unit.
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
    report("zero", greedy_coin_count(0) == 0);

    {
        const bool ok = greedy_coin_count(1) == 1 && greedy_coin_count(2) == 1 &&
                        greedy_coin_count(3) == 2 && greedy_coin_count(4) == 2;
        report("small", ok);
    }

    {
        const bool ok = greedy_coin_count(99) == 6 && greedy_coin_count(100) == 1 &&
                        greedy_coin_count(199) == 7 && greedy_coin_count(1000) == 5;
        report("boundary", ok);
    }

    {
        const bool ok = greedy_coin_count(30) == 2 && greedy_coin_count(60) == 2 &&
                        greedy_coin_count(80) == 3 && greedy_coin_count(90) == 3;
        report("greedy", ok);
    }

    report("large", greedy_coin_count(1000000000000000000LL) == 5000000000000000LL);
    report("near-max", greedy_coin_count(999999999999999999LL) == 5000000000000006LL);

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
