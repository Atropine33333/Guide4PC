// 参考实现：types-exact-int
#include <cmath>
#include <limits>

bool to_int_exact(double value, int& out) {
    if (!std::isfinite(value)) return false;
    if (value != std::trunc(value)) return false;

    const double lowest = static_cast<double>(std::numeric_limits<int>::min());
    const double highest = static_cast<double>(std::numeric_limits<int>::max());
    if (value < lowest || value > highest) return false;

    out = static_cast<int>(value);
    return true;
}
