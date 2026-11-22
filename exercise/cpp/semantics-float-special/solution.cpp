#include <cmath>

bool is_nan_value(double x) { return std::isnan(x); }

bool is_finite_value(double x) { return std::isfinite(x); }

bool is_negative_zero(double x) { return x == 0.0 && std::signbit(x); }

bool same_value(double a, double b) { return a == b; }

int total_order(double a, double b) {
    if (a < b) return -1;
    if (a > b) return 1;
    if (a == b) return 0;
    return 2;
}

double safe_div(double a, double b, double fallback) {
    if (b == 0.0) {
        return fallback;
    }
    return a / b;
}
