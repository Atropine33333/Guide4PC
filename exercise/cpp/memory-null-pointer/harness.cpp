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

}  // namespace

int main() {
    report("null", sum_or_zero(nullptr, 5) == 0 &&
                       max_or(nullptr, 5, -7) == -7 &&
                       count_even(nullptr, 5) == 0);

    {
        const int values[] = {2, -4, 7};
        report("basic", sum_or_zero(values, 3) == 5 &&
                            max_or(values, 3, 0) == 7 &&
                            count_even(values, 3) == 2);
    }

    {
        const int values[] = {-9, -3, -1};
        report("negative", sum_or_zero(values, 3) == -13 &&
                               max_or(values, 3, 0) == -1 &&
                               count_even(values, 3) == 0);
    }

    {
        const int values[] = {0};
        report("single", sum_or_zero(values, 1) == 0 &&
                             max_or(values, 1, 42) == 0 &&
                             count_even(values, 1) == 1);
    }

    {
        const int values[] = {8, 1};
        report("non-positive", sum_or_zero(values, 0) == 0 &&
                                   max_or(values, 0, 33) == 33 &&
                                   count_even(values, 0) == 0 &&
                                   sum_or_zero(values, -3) == 0 &&
                                   max_or(values, -3, 33) == 33 &&
                                   count_even(values, -3) == 0);
    }

    {
        std::vector<int> values(100000);
        long long expected_sum = 0;
        int expected_max = values[0];
        int expected_even = 0;
        for (int i = 0; i < 100000; ++i) {
            values[static_cast<std::size_t>(i)] = i - 50000;
            expected_sum += i - 50000;
            if (i - 50000 > expected_max) expected_max = i - 50000;
            if ((i - 50000) % 2 == 0) ++expected_even;
        }
        report("large", sum_or_zero(values.data(), 100000) == expected_sum &&
                           max_or(values.data(), 100000, 0) == expected_max &&
                           count_even(values.data(), 100000) == expected_even);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
