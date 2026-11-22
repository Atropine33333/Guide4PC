// 参考实现：types-safe-add
#include <limits>

namespace {

bool fits(long long result, int& out) {
    if (result < static_cast<long long>(std::numeric_limits<int>::min()) ||
        result > static_cast<long long>(std::numeric_limits<int>::max())) {
        return false;
    }
    out = static_cast<int>(result);
    return true;
}

}  // namespace

bool add_ints(int a, int b, int& out) {
    return fits(static_cast<long long>(a) + static_cast<long long>(b), out);
}

bool sub_ints(int a, int b, int& out) {
    return fits(static_cast<long long>(a) - static_cast<long long>(b), out);
}

bool mul_ints(int a, int b, int& out) {
    return fits(static_cast<long long>(a) * static_cast<long long>(b), out);
}
