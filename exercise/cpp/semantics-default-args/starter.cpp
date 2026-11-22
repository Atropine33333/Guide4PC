#include <string>
#include <vector>

int clamp_value(int value, int low = 0, int high = 100) {
    (void)low;
    (void)high;
    // TODO: 截断到 [low, high]，并处理 low > high 的情况。
    return value;
}

std::string join_text(const std::vector<std::string>& parts,
                      const std::string& separator = ",") {
    (void)separator;
    if (parts.empty()) return "";
    // TODO: 用分隔符连接所有元素。
    return parts[0];
}
