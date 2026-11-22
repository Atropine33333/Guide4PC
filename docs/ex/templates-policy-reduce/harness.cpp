// Guide4PC harness appended to the student's code in the same translation unit.
#include <climits>
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

struct ProductPolicy {
    static int combine(int left, int right) { return left * right; }
    static int identity() { return 1; }
};

struct MinPolicy {
    static int combine(int left, int right) {
        return left < right ? left : right;
    }
    static int identity() { return INT_MAX; }
};

}  // namespace

int main() {
    {
        const std::vector<int> values = {1, 2, 3};
        report("sum-basic", reduce<SumPolicy>(values) == 6);
    }

    {
        const std::vector<int> empty;
        const std::vector<int> single = {42};
        report("sum-empty-and-single",
               reduce<SumPolicy>(empty) == 0 &&
                   reduce<SumPolicy>(single) == 42);
    }

    {
        const std::vector<int> values = {3, 9, 2, 9};
        report("max-basic", reduce<MaxPolicy>(values) == 9);
    }

    {
        const std::vector<int> values = {-5, -2, -9};
        const std::vector<int> empty;
        report("max-negative-and-empty",
               reduce<MaxPolicy>(values) == -2 &&
                   reduce<MaxPolicy>(empty) == INT_MIN);
    }

    {
        const std::vector<int> values = {2, 3, 4};
        const std::vector<int> empty;
        report("custom-policy", reduce<ProductPolicy>(values) == 24 &&
                                   reduce<ProductPolicy>(empty) == 1);
    }

    {
        const std::vector<int> values = {7, 7, 7, 1, 9, 4};
        report("custom-min", reduce<MinPolicy>(values) == 1);
    }

    {
        std::vector<int> values;
        long long expected = 0;
        for (int i = 1; i <= 1000; ++i) {
            values.push_back(i);
            expected += i;
        }
        report("large-sum", reduce<SumPolicy>(values) == expected);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
