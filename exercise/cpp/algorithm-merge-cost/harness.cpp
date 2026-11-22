// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
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

struct h_rng {
    std::uint32_t state;
    std::uint32_t next() {
        state = state * 1664525u + 1013904223u;
        return state >> 8;
    }
    int below(int limit) { return static_cast<int>(next() % static_cast<std::uint32_t>(limit)); }
};

long long h_brute(std::vector<long long> piles) {
    if (piles.size() < 2) return 0;
    long long best = LLONG_MAX;
    for (std::size_t i = 0; i + 1 < piles.size(); ++i) {
        std::vector<long long> next;
        next.reserve(piles.size() - 1);
        for (std::size_t j = 0; j < piles.size(); ++j) {
            if (j == i) {
                next.push_back(piles[j] + piles[j + 1]);
            } else if (j == i + 1) {
                continue;
            } else {
                next.push_back(piles[j]);
            }
        }
        const long long cost = piles[i] + piles[i + 1] + h_brute(next);
        if (cost < best) best = cost;
    }
    return best;
}

}  // namespace

int main() {
    {
        report("single", min_merge_cost({7}) == 0 && min_merge_cost({}) == 0);
    }

    {
        report("two", min_merge_cost({4, 9}) == 13 && min_merge_cost({1000000, 1000000}) == 2000000);
    }

    {
        report("three", min_merge_cost({1, 2, 3}) == 9 && min_merge_cost({10, 1, 10}) == 32);
    }

    {
        report("equal", min_merge_cost({4, 4, 4, 4}) == 32);
    }

    {
        report("increasing", min_merge_cost({1, 2, 3, 4, 5}) == 33);
    }

    {
        h_rng rng{202411u};
        bool ok = true;
        for (int trial = 0; trial < 20 && ok; ++trial) {
            const int n = 2 + rng.below(6);
            std::vector<int> piles;
            std::vector<long long> wide;
            for (int i = 0; i < n; ++i) {
                const int value = 1 + rng.below(20);
                piles.push_back(value);
                wide.push_back(value);
            }
            if (min_merge_cost(piles) != h_brute(wide)) ok = false;
        }
        report("brute-force", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
