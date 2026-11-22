// 参考实现：types-near-equal
#include <cmath>

bool nearly_equal(double a, double b, double tolerance) {
    if (a == b) return true;
    if (!std::isfinite(a) || !std::isfinite(b)) return false;
    return std::fabs(a - b) <= tolerance;
}
