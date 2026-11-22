// 参考实现：algorithm-edit-distance
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

int edit_distance(const std::string& a, const std::string& b) {
    const std::size_t n = a.size();
    const std::size_t m = b.size();
    std::vector<int> prev(m + 1);
    std::vector<int> cur(m + 1);
    for (std::size_t j = 0; j <= m; ++j) prev[j] = static_cast<int>(j);
    for (std::size_t i = 1; i <= n; ++i) {
        cur[0] = static_cast<int>(i);
        for (std::size_t j = 1; j <= m; ++j) {
            const int substitute = prev[j - 1] + (a[i - 1] == b[j - 1] ? 0 : 1);
            cur[j] = std::min({prev[j] + 1, cur[j - 1] + 1, substitute});
        }
        prev.swap(cur);
    }
    return prev[m];
}

int longest_common_subsequence(const std::string& a, const std::string& b) {
    const std::size_t n = a.size();
    const std::size_t m = b.size();
    std::vector<int> prev(m + 1, 0);
    std::vector<int> cur(m + 1, 0);
    for (std::size_t i = 1; i <= n; ++i) {
        cur[0] = 0;
        for (std::size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                cur[j] = prev[j - 1] + 1;
            } else {
                cur[j] = std::max(prev[j], cur[j - 1]);
            }
        }
        prev.swap(cur);
    }
    return prev[m];
}
