// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <random>

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
    report("zero", sum_all() == 0);

    {
        const bool ok = sum_all(1, 2, 3, 4, 5) == 15 &&
                        sum_all(7) == 7 && sum_all(-1) == -1;
        report("ints", ok);
    }

    {
        const bool ok = sum_all('a', 2, 3L) == 102 &&
                        sum_all('A', 'B') == 131;
        report("mixed", ok);
    }

    {
        const bool ok = sum_all(-5, -10, 2) == -13 &&
                        sum_all(0, 0, 0, 0) == 0;
        report("negatives", ok);
    }

    {
        const bool ok = sum_all(10000000000LL, 1LL) == 10000000001LL &&
                        sum_all(4294967296LL, -4294967296LL) == 0;
        report("longlong", ok);
    }

    {
        long long expected = 0;
        for (int i = 1; i <= 100; ++i) expected += i;
        const bool ok = sum_all(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 55 &&
                        sum_all(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15) == 120 &&
                        expected == 5050;
        report("many", ok);
    }

    {
        std::mt19937 rng(20240517u);
        std::uniform_int_distribution<int> distribution(-1000, 1000);
        bool ok = true;
        for (int round = 0; round < 100 && ok; ++round) {
            const int a = distribution(rng);
            const int b = distribution(rng);
            const int c = distribution(rng);
            const int d = distribution(rng);
            const int e = distribution(rng);
            const int f = distribution(rng);
            const long long expected = static_cast<long long>(a) + b + c + d + e + f;
            if (sum_all(a, b, c, d, e, f) != expected) ok = false;
        }
        report("random", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
