// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <limits>

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
    const double inf = std::numeric_limits<double>::infinity();
    const double nan = std::numeric_limits<double>::quiet_NaN();

    {
        const bool ok = nearly_equal(1.5, 1.5, 0.0) &&
                        nearly_equal(0.0, -0.0, 0.0) &&
                        nearly_equal(123456.789, 123456.789, 0.0);
        report("identical", ok);
    }

    {
        const bool ok = nearly_equal(1.0, 1.0000001, 1e-5) &&
                        nearly_equal(0.1 + 0.2, 0.3, 1e-9) &&
                        nearly_equal(-1.0, -1.0000001, 1e-5);
        report("within", ok);
    }

    {
        const bool ok = !nearly_equal(1.0, 2.0, 0.5) &&
                        !nearly_equal(1.0, 1.5, 0.1) &&
                        !nearly_equal(-1.0, 1.0, 1.999);
        report("outside", ok);
    }

    {
        const bool ok = nearly_equal(1.0, 1.0, 0.0) &&
                        !nearly_equal(1.0, 1.0000000000000002, 0.0);
        report("zero-tolerance", ok);
    }

    {
        const bool ok = nearly_equal(inf, inf, 0.0) &&
                        nearly_equal(-inf, -inf, 0.0) &&
                        !nearly_equal(inf, -inf, 0.0) &&
                        !nearly_equal(inf, 1e308, 1e308);
        report("infinity", ok);
    }

    {
        const bool ok = !nearly_equal(nan, nan, 1.0) &&
                        !nearly_equal(nan, 1.0, 1.0) &&
                        !nearly_equal(1.0, nan, 1.0);
        report("nan", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
