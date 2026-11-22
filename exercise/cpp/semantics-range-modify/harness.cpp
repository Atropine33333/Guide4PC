// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
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

bool same(const std::vector<int>& values, const std::vector<int>& expected) {
    return values == expected;
}

}  // namespace

int main() {
    {
        std::vector<int> values{1, 2, 3};
        scale_in_place(values, 2);
        report("scale-basic", same(values, {2, 4, 6}));
    }

    {
        std::vector<int> values{-1, 0, 5};
        scale_in_place(values, -3);
        report("scale-negative", same(values, {3, 0, -15}));
    }

    {
        std::vector<int> values{4, -2};
        scale_in_place(values, 0);
        report("scale-zero", same(values, {0, 0}));
    }

    {
        std::vector<int> values;
        scale_in_place(values, 7);
        report("scale-empty", values.empty());
    }

    {
        const std::vector<int> mixed{1, -2, 3, 0, -4};
        const std::vector<int> none{-1, -2, -3};
        const std::vector<int> huge{2147483647, 2147483647};
        const std::vector<int> empty;
        const bool ok = sum_positive(mixed) == 4 && sum_positive(none) == 0 &&
                        sum_positive(empty) == 0 &&
                        sum_positive(huge) == 4294967294LL;
        report("sum-positive", ok);
    }

    {
        const std::vector<int> mixed{3, 6, 7, 0, -9};
        const std::vector<int> negatives{2, 3, -4};
        const std::vector<int> all{1, 2, 3};
        const std::vector<int> empty;
        const bool ok = count_multiples(mixed, 3) == 4 &&
                        count_multiples(negatives, -2) == 2 &&
                        count_multiples(empty, 5) == 0 &&
                        count_multiples(all, 1) == 3;
        report("multiples", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
