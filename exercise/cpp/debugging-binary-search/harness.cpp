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
    report("empty", lower_bound_index(std::vector<int>{}, 3) == 0);

    {
        const std::vector<int> values{5};
        const bool ok = lower_bound_index(values, 3) == 0 &&
                        lower_bound_index(values, 5) == 0 &&
                        lower_bound_index(values, 7) == 1;
        report("single", ok);
    }

    {
        const std::vector<int> values{1, 3, 3, 3, 7};
        const bool ok = lower_bound_index(values, 3) == 1 &&
                        lower_bound_index(values, 7) == 4 &&
                        lower_bound_index(values, 0) == 0 &&
                        lower_bound_index(values, 9) == 5;
        report("duplicates", ok);
    }

    {
        const std::vector<int> values{1, 2, 4, 8};
        const bool ok = lower_bound_index(values, 4) == 2 &&
                        lower_bound_index(values, 5) == 3 &&
                        lower_bound_index(values, 8) == 3;
        report("missing", ok);
    }

    {
        const std::vector<int> values{-10, -5, -1, 0};
        const bool ok = lower_bound_index(values, -10) == 0 &&
                        lower_bound_index(values, -7) == 1 &&
                        lower_bound_index(values, 1) == 4;
        report("negative", ok);
    }

    {
        std::vector<int> values;
        for (int i = 0; i <= 100; ++i) values.push_back(i * 2);
        const bool ok = lower_bound_index(values, 0) == 0 &&
                        lower_bound_index(values, 51) == 26 &&
                        lower_bound_index(values, 200) == 100 &&
                        lower_bound_index(values, 201) == 101;
        report("large", ok);
    }

    {
        const std::vector<int> values{4, 4, 4, 4};
        const bool ok = lower_bound_index(values, 4) == 0 &&
                        lower_bound_index(values, 3) == 0 &&
                        lower_bound_index(values, 5) == 4;
        report("all-equal", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
