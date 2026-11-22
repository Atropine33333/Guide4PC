#include <string>

std::string format_duration(long long total_seconds) {
    // TODO: 按 H:MM:SS 格式化，分钟和秒要补足两位。
    return std::to_string(total_seconds);
}
