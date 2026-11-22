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

long long naive(const std::vector<int>& values) {
    long long count = 0;
    for (std::size_t i = 0; i < values.size(); ++i) {
        for (std::size_t j = i + 1; j < values.size(); ++j) {
            if (values[i] > values[j]) ++count;
        }
    }
    return count;
}

}  // namespace

int main() {
    {
        const bool ok = count_inversions({}) == 0 && count_inversions({7}) == 0 &&
                        count_inversions({1, 2, 3, 4}) == 0 &&
                        count_inversions({2, 2, 2}) == 0;
        report("trivial", ok);
    }

    {
        const bool ok = count_inversions({5, 4, 3, 2, 1}) == 10 &&
                        count_inversions({1, 2, 3, 4, 5}) == 0;
        report("sorted-reversed", ok);
    }

    {
        const bool ok = count_inversions({2, 2, 1}) == 2 &&
                        count_inversions({3, 1, 3, 1}) == 3 &&
                        count_inversions({1, 1, 2, 1}) == 1;
        report("duplicates", ok);
    }

    {
        const bool ok = count_inversions({-1, -2, -3}) == 3 &&
                        count_inversions({0, -5, 5, -5}) == 3 &&
                        count_inversions({-3, -2, -1, 0}) == 0;
        report("negatives", ok);
    }

    {
        std::vector<int> descending(100000);
        for (int i = 0; i < 100000; ++i) descending[i] = 100000 - i;
        const bool ok = count_inversions(descending) == 4999950000LL;
        report("large", ok);
    }

    {
        std::mt19937 rng(424242u);
        std::uniform_int_distribution<int> distribution(-1000, 1000);
        bool ok = true;
        for (int round = 0; round < 20 && ok; ++round) {
            std::vector<int> values(200);
            for (int& value : values) value = distribution(rng);
            if (count_inversions(values) != naive(values)) ok = false;
        }
        report("random", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
