// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

int reference_lis(const std::vector<int> &values) {
    const std::size_t n = values.size();
    std::vector<int> dp(n, 1);
    int best = 0;
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < i; ++j) {
            if (values[j] < values[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
        best = std::max(best, dp[i]);
    }
    return best;
}

}  // namespace

int main() {
    report("empty", lis_length({}) == 0);

    report("single", lis_length({7}) == 1 && lis_length({-3}) == 1);

    {
        std::vector<int> increasing(1000);
        for (int i = 0; i < 1000; ++i) increasing[static_cast<std::size_t>(i)] = i;
        report("increasing", lis_length(increasing) == 1000);
    }

    {
        std::vector<int> decreasing(1000);
        for (int i = 0; i < 1000; ++i) decreasing[static_cast<std::size_t>(i)] = 1000 - i;
        report("decreasing", lis_length(decreasing) == 1);
    }

    report("duplicates", lis_length({1, 1, 2, 2, 3}) == 3 &&
                             lis_length({2, 2, 2}) == 1 &&
                             lis_length({1, 2, 1, 2, 1, 2}) == 2);

    report("classic", lis_length({10, 9, 2, 5, 3, 7, 101, 18}) == 4 &&
                          lis_length({0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15}) == 6);

    {
        std::mt19937 rng(20261122u);
        std::uniform_int_distribution<int> dist(-50, 50);
        std::vector<int> values(300);
        for (int &value : values) value = dist(rng);
        report("random", lis_length(values) == reference_lis(values));
    }

    {
        std::vector<int> large(200000);
        for (int i = 0; i < 200000; ++i) large[static_cast<std::size_t>(i)] = i * 2;
        report("large", lis_length(large) == 200000);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
