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

const int kIntMax = std::numeric_limits<int>::max();
const int kIntMin = std::numeric_limits<int>::min();
const double kNan = std::numeric_limits<double>::quiet_NaN();
const double kInf = std::numeric_limits<double>::infinity();

}  // namespace

int main() {
    report("int-in-range",
           saturate_to_int(0) == 0 && saturate_to_int(42) == 42 &&
               saturate_to_int(-42) == -42 && saturate_to_int(1000000) == 1000000);

    report("int-boundaries",
           saturate_to_int(static_cast<long long>(kIntMax)) == kIntMax &&
               saturate_to_int(static_cast<long long>(kIntMin)) == kIntMin &&
               saturate_to_int(static_cast<long long>(kIntMax) + 1) == kIntMax &&
               saturate_to_int(static_cast<long long>(kIntMin) - 1) == kIntMin);

    report("long-long-extremes",
           saturate_to_int(std::numeric_limits<long long>::max()) == kIntMax &&
               saturate_to_int(std::numeric_limits<long long>::min()) == kIntMin);

    report("short-boundaries",
           saturate_to_short(32767) == 32767 && saturate_to_short(-32768) == -32768 &&
               saturate_to_short(32768) == 32767 && saturate_to_short(-32769) == -32768 &&
               saturate_to_short(70000) == 32767 &&
               saturate_to_short(std::numeric_limits<long long>::min()) == -32768);

    report("double-truncation",
           saturate_double_to_int(3.9) == 3 && saturate_double_to_int(-3.9) == -3 &&
               saturate_double_to_int(-0.0) == 0 &&
               saturate_double_to_int(2147483646.75) == 2147483646);

    report("double-nan", saturate_double_to_int(kNan) == 0);

    report("double-infinity-and-range",
           saturate_double_to_int(kInf) == kIntMax &&
               saturate_double_to_int(-kInf) == kIntMin &&
               saturate_double_to_int(1e100) == kIntMax &&
               saturate_double_to_int(-1e100) == kIntMin &&
               saturate_double_to_int(2147483648.0) == kIntMax &&
               saturate_double_to_int(-2147483649.0) == kIntMin);

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
