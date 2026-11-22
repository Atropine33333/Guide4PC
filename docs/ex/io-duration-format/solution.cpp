// 参考实现：io-duration-format
#include <string>

std::string format_duration(long long total_seconds) {
    const long long hours = total_seconds / 3600;
    const int minutes = static_cast<int>((total_seconds / 60) % 60);
    const int seconds = static_cast<int>(total_seconds % 60);

    std::string result = std::to_string(hours);
    result += ':';
    result += static_cast<char>('0' + minutes / 10);
    result += static_cast<char>('0' + minutes % 10);
    result += ':';
    result += static_cast<char>('0' + seconds / 10);
    result += static_cast<char>('0' + seconds % 10);
    return result;
}
