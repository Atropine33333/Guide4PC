// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
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

}  // namespace

int main() {
    {
        std::vector<int> values;
        for (int value : IntRange(0, 5)) values.push_back(value);
        report("collect", values == std::vector<int>({0, 1, 2, 3, 4}));
    }

    {
        const IntRange same(3, 3);
        const IntRange reversed(5, 2);
        report("empty", same.begin() == same.end() &&
                            reversed.begin() == reversed.end() &&
                            std::distance(same.begin(), same.end()) == 0);
    }

    {
        std::vector<int> values;
        for (int value : IntRange(-3, 2)) values.push_back(value);
        report("negative", values == std::vector<int>({-3, -2, -1, 0, 1}));
    }

    {
        const IntRange one(7, 8);
        report("single", std::distance(one.begin(), one.end()) == 1 &&
                             *one.begin() == 7);
    }

    {
        const IntRange range(1, 11);
        const int sum = std::accumulate(range.begin(), range.end(), 0);
        const std::vector<int> values(range.begin(), range.end());
        report("algorithms", sum == 55 && values.size() == 10 &&
                                 values.front() == 1 && values.back() == 10);
    }

    {
        IntRange range(2, 6);
        auto it = range.begin();
        const int first = *it;
        auto old = it++;
        const bool post = old == range.begin() && it == ++range.begin();
        bool compatible = true;
        std::vector<int> values;
        for (; it != range.end(); ++it) values.push_back(*it);
        if (first != 2 || !post || values != std::vector<int>({3, 4, 5})) {
            compatible = false;
        }
        report("iterator", compatible);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
