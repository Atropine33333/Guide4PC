// 参考实现：semantics-default-args
#include <string>
#include <vector>

int clamp_value(int value, int low = 0, int high = 100) {
    if (low > high) {
        const int temporary = low;
        low = high;
        high = temporary;
    }
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

std::string join_text(const std::vector<std::string>& parts,
                      const std::string& separator = ",") {
    std::string result;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) result += separator;
        result += parts[i];
    }
    return result;
}
