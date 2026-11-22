// Guide4PC harness appended to the student's code in the same translation unit.
#include <algorithm>
#include <iostream>
#include <random>
#include <set>
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

}  // namespace

int main() {
    report("empty", count_distinct({}) == 0);
    report("single", count_distinct({5}) == 1 && count_distinct({-7}) == 1);
    report("same", count_distinct({3, 3, 3, 3}) == 1 &&
                       count_distinct({-1, -1}) == 1);
    report("all-distinct", count_distinct({1, 2, 3, 4}) == 4 &&
                               count_distinct({-3, -2, -1, 0, 1}) == 5);
    report("mixed", count_distinct({1, 1, 2, 3, 3, 3, 5}) == 4 &&
                        count_distinct({-5, -5, 0, 0, 2}) == 3);

    {
        std::mt19937 rng(20261122u);
        std::uniform_int_distribution<int> dist(-10, 10);
        std::vector<int> values(500);
        for (int &value : values) value = dist(rng);
        std::sort(values.begin(), values.end());
        const int expected = static_cast<int>(
            std::set<int>(values.begin(), values.end()).size());
        report("random", count_distinct(values) == expected);
    }

    {
        std::vector<int> values;
        values.reserve(100000);
        for (int i = 0; i < 50000; ++i) {
            values.push_back(i);
            values.push_back(i);
        }
        report("large", count_distinct(values) == 50000);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
