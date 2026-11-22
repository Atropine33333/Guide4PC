// Guide4PC harness appended to the student's code in the same translation unit.
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

std::vector<int> collect(const IntRange& range) {
    std::vector<int> values;
    for (int value : range) {
        values.push_back(value);
    }
    return values;
}

}  // namespace

int main() {
    {
        const IntRange range(0, 5);
        report("positive-step", collect(range) == std::vector<int>{0, 1, 2, 3, 4});
    }

    {
        const IntRange range(1, 10, 3);
        report("step-three", collect(range) == std::vector<int>{1, 4, 7});
    }

    {
        const IntRange range(5, 0, -2);
        report("negative-step", collect(range) == std::vector<int>{5, 3, 1});
    }

    {
        const bool ok = collect(IntRange(3, 3)).empty() &&
                        collect(IntRange(5, 0)).empty() &&
                        collect(IntRange(0, 5, -1)).empty() &&
                        collect(IntRange(0, 6, 2)) == std::vector<int>{0, 2, 4};
        report("empty-and-divisible", ok);
    }

    {
        const bool ok = collect(IntRange(7, 8)) == std::vector<int>{7} &&
                        collect(IntRange(7, 6, -1)) == std::vector<int>{7} &&
                        collect(IntRange(-2, 1)) == std::vector<int>{-2, -1, 0};
        report("single-and-negative", ok);
    }

    {
        const IntRange range(0, 4);
        IntRange::iterator first = range.begin();
        IntRange::iterator copy = first;
        bool ok = first == copy && first != range.end() && *first == 0;
        ++first;
        ok = ok && *first == 1 && *copy == 0 && first == copy == false;
        report("iterator-copy", ok);
    }

    {
        long long sum = 0;
        for (int value : IntRange(0, 1000)) {
            sum += value;
        }
        report("large-range", sum == 499500);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
