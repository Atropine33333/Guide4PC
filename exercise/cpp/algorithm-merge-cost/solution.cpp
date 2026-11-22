// 参考实现：algorithm-merge-cost
#include <algorithm>
#include <cstddef>
#include <limits>
#include <vector>

long long min_merge_cost(const std::vector<int>& piles) {
    const std::size_t n = piles.size();
    if (n < 2) return 0;

    std::vector<long long> prefix(n + 1, 0);
    for (std::size_t i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + piles[i];

    std::vector<std::vector<long long>> dp(n, std::vector<long long>(n, 0));
    for (std::size_t len = 2; len <= n; ++len) {
        for (std::size_t i = 0; i + len <= n; ++i) {
            const std::size_t j = i + len - 1;
            long long best = std::numeric_limits<long long>::max();
            for (std::size_t k = i; k < j; ++k) {
                best = std::min(best, dp[i][k] + dp[k + 1][j]);
            }
            dp[i][j] = best + prefix[j + 1] - prefix[i];
        }
    }
    return dp[0][n - 1];
}
