// Guide4PC harness appended to the student's code in the same translation unit.
#include <cmath>
#include <iostream>
#include <limits>
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
    {
        int out = 0;
        const bool ok = to_int_exact(0.0, out) && out == 0 &&
                        to_int_exact(1.0, out) && out == 1 &&
                        to_int_exact(-1.0, out) && out == -1 &&
                        to_int_exact(-0.0, out) && out == 0 &&
                        to_int_exact(42.0, out) && out == 42;
        report("integral", ok);
    }

    {
        int out = 777;
        const bool first = !to_int_exact(0.5, out) && out == 777;
        const bool second = !to_int_exact(-7.25, out) && out == 777;
        const bool third = !to_int_exact(3.0000001, out) && out == 777;
        report("fractional", first && second && third);
    }

    {
        int out = 0;
        const bool ok = to_int_exact(2147483647.0, out) && out == 2147483647 &&
                        !to_int_exact(2147483648.0, out) && out == 2147483647 &&
                        !to_int_exact(1e20, out) && out == 2147483647;
        report("positive-bound", ok);
    }

    {
        int out = 0;
        const bool ok = to_int_exact(-2147483648.0, out) && out == -2147483647 - 1 &&
                        !to_int_exact(-2147483649.0, out) && out == -2147483647 - 1 &&
                        !to_int_exact(-1e20, out) && out == -2147483647 - 1;
        report("negative-bound", ok);
    }

    {
        int out = 123456;
        const double nan_value = std::numeric_limits<double>::quiet_NaN();
        const double inf_value = std::numeric_limits<double>::infinity();
        const bool ok = !to_int_exact(nan_value, out) && out == 123456 &&
                        !to_int_exact(inf_value, out) && out == 123456 &&
                        !to_int_exact(-inf_value, out) && out == 123456;
        report("special", ok);
    }

    {
        int out = -99;
        const bool ok = to_int_exact(1.0, out) && out == 1 &&
                        !to_int_exact(1.5, out) && out == 1 &&
                        to_int_exact(-5.0, out) && out == -5 &&
                        !to_int_exact(3e9, out) && out == -5;
        report("unchanged", ok);
    }

    {
        std::mt19937 rng(20240517u);
        bool ok = true;
        for (int i = 0; i < 200 && ok; ++i) {
            const int value = static_cast<int>(rng());
            int out = 0;
            if (!to_int_exact(static_cast<double>(value), out) || out != value) {
                ok = false;
            } else {
                const double fractional = static_cast<double>(value) + 0.5;
                int untouched = 31;
                if (to_int_exact(fractional, untouched) || untouched != 31) {
                    ok = false;
                }
            }
        }
        report("random", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
