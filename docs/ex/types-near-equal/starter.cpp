#include <cmath>

bool nearly_equal(double a, double b, double tolerance) {
    // TODO: inf - inf 会得到 NaN，这个写法无法识别同号无穷。
    return std::fabs(a - b) <= tolerance;
}
