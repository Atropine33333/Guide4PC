#include <cmath>
#include <limits>

int saturate_to_int(long long x) {
    if (x > static_cast<long long>(std::numeric_limits<int>::max())) {
        return std::numeric_limits<int>::max();
    }
    if (x < static_cast<long long>(std::numeric_limits<int>::min())) {
        return std::numeric_limits<int>::min();
    }
    return static_cast<int>(x);
}

short saturate_to_short(long long x) {
    if (x > static_cast<long long>(std::numeric_limits<short>::max())) {
        return std::numeric_limits<short>::max();
    }
    if (x < static_cast<long long>(std::numeric_limits<short>::min())) {
        return std::numeric_limits<short>::min();
    }
    return static_cast<short>(x);
}

int saturate_double_to_int(double x) {
    if (std::isnan(x)) {
        return 0;
    }
    if (x >= static_cast<double>(std::numeric_limits<int>::max())) {
        return std::numeric_limits<int>::max();
    }
    if (x <= static_cast<double>(std::numeric_limits<int>::min())) {
        return std::numeric_limits<int>::min();
    }
    return static_cast<int>(x);
}
