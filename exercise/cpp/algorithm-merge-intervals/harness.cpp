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

bool same(const std::vector<Interval>& actual, const std::vector<Interval>& expected) {
    if (actual.size() != expected.size()) return false;
    for (std::size_t i = 0; i < actual.size(); ++i) {
        if (actual[i].left != expected[i].left || actual[i].right != expected[i].right) {
            return false;
        }
    }
    return true;
}

}  // namespace

int main() {
    report("empty", merge_intervals({}).empty());

    {
        const auto result = merge_intervals({{1, 3}});
        report("single", same(result, {{1, 3}}));
    }

    {
        const auto result = merge_intervals({{4, 5}, {1, 2}});
        report("disjoint", same(result, {{1, 2}, {4, 5}}));
    }

    {
        const auto result = merge_intervals({{5, 7}, {1, 3}, {2, 4}});
        report("overlapping-unsorted", same(result, {{1, 4}, {5, 7}}));
    }

    {
        const auto result = merge_intervals({{1, 10}, {3, 4}, {11, 12}});
        report("contained", same(result, {{1, 10}, {11, 12}}));
    }

    {
        const auto result = merge_intervals({{1, 2}, {1, 2}, {2, 3}});
        report("duplicates-touching", same(result, {{1, 3}}));
    }

    {
        const auto result = merge_intervals({{-5, -3}, {-4, -1}, {0, 2}});
        report("negative", same(result, {{-5, -1}, {0, 2}}));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
