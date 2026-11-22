// Guide4PC harness appended to the student's code in the same translation unit.
#include <cmath>
#include <iostream>
#include <limits>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

const double kNan = std::numeric_limits<double>::quiet_NaN();
const double kInf = std::numeric_limits<double>::infinity();

}  // namespace

int main() {
    {
        const bool ok = is_nan_value(kNan) && !is_nan_value(1.0) &&
                        !is_nan_value(kInf) && !is_nan_value(-0.0);
        report("nan-detection", ok);
    }

    {
        const bool ok = is_finite_value(1.5) && is_finite_value(-0.0) &&
                        !is_finite_value(kInf) && !is_finite_value(-kInf) &&
                        !is_finite_value(kNan);
        report("finite-detection", ok);
    }

    {
        const bool ok = is_negative_zero(-0.0) && !is_negative_zero(0.0) &&
                        !is_negative_zero(-1e-300) && !is_negative_zero(kNan);
        report("negative-zero", ok);
    }

    {
        const bool ok = same_value(2.5, 2.5) && same_value(-0.0, 0.0) &&
                        !same_value(kNan, kNan) && !same_value(kNan, 1.0);
        report("same-value", ok);
    }

    {
        const bool ok = total_order(1.0, 2.0) == -1 && total_order(2.0, 1.0) == 1 &&
                        total_order(2.0, 2.0) == 0 && total_order(-kInf, 1.0) == -1 &&
                        total_order(kInf, kInf) == 0 && total_order(-0.0, 0.0) == 0;
        report("total-order", ok);
    }

    {
        const bool ok = total_order(kNan, 1.0) == 2 && total_order(1.0, kNan) == 2 &&
                        total_order(kNan, kNan) == 2;
        report("unordered", ok);
    }

    {
        const bool ok = safe_div(6.0, 3.0, 99.0) == 2.0 &&
                        safe_div(1.0, 0.0, 7.5) == 7.5 &&
                        safe_div(1.0, -0.0, 7.5) == 7.5 &&
                        safe_div(0.0, 0.0, -3.0) == -3.0 &&
                        safe_div(1.0, kInf, 9.0) == 0.0;
        report("safe-div", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
