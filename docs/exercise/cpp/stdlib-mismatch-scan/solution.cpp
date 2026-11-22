#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

std::size_t common_prefix(const std::string& a, const std::string& b) {
    const auto mismatch = std::mismatch(a.begin(), a.end(), b.begin(), b.end());
    return static_cast<std::size_t>(mismatch.first - a.begin());
}

long long last_index_of(const std::vector<int>& values, int target) {
    for (std::size_t i = values.size(); i-- > 0;) {
        if (values[i] == target) {
            return static_cast<long long>(i);
        }
    }
    return -1;
}

bool is_rotation(const std::string& a, const std::string& b) {
    return a.size() == b.size() && (a + a).find(b) != std::string::npos;
}
