// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool same(const std::vector<long long>& actual,
          const std::vector<long long>& expected) {
    return actual == expected;
}

}  // namespace

int main() {
    report("basic", same(sliding_max({1, 3, -1, -3, 5, 3, 6, 7}, 3),
                         {3, 3, 5, 5, 6, 7}) &&
                        same(sliding_max({9, 8, 7, 6}, 2), {9, 8, 7}));

    report("sizes", same(sliding_max({4, -2, 7}, 1), {4, -2, 7}) &&
                        same(sliding_max({4, -2, 7}, 3), {7}) &&
                        same(sliding_max({5}, 1), {5}));

    report("invalid", sliding_max({1, 2, 3}, 0).empty() &&
                          sliding_max({1, 2, 3}, 4).empty() &&
                          sliding_max({}, 0).empty() &&
                          sliding_max({}, 1).empty());

    report("negatives", same(sliding_max({-5, -5, -2, -9, -1}, 2),
                             {-5, -2, -2, -1}) &&
                            same(sliding_max({0, 0, 0}, 2), {0, 0}) &&
                            same(sliding_max({2, 1, 2, 1, 2}, 2), {2, 2, 2, 2}));

    report("monotone", same(sliding_max({1, 2, 3, 4, 5}, 3), {3, 4, 5}) &&
                           same(sliding_max({5, 4, 3, 2, 1}, 3), {5, 4, 3}) &&
                           same(sliding_max({1, 3, 2, 4, 3, 5}, 3), {3, 4, 4, 5}));

    {
        std::vector<long long> values;
        for (int i = 0; i < 10000; ++i) {
            values.push_back(static_cast<long long>((i * 37) % 101) - 50);
        }
        const std::size_t window = 7;
        std::vector<long long> expected;
        for (std::size_t i = 0; i + window <= values.size(); ++i) {
            long long best = values[i];
            for (std::size_t j = i + 1; j < i + window; ++j) {
                if (values[j] > best) best = values[j];
            }
            expected.push_back(best);
        }
        report("long", same(sliding_max(values, window), expected));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
