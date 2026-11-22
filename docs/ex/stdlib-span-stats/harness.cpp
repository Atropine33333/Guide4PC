// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <span>
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

bool same_stats(const Stats& s, std::size_t count, long long sum, int min, int max) {
    return s.count == count && s.sum == sum && s.min == min && s.max == max;
}

}  // namespace

int main() {
    {
        const std::vector<int> empty;
        report("empty-stats", same_stats(summarize(empty), 0, 0, 0, 0));
    }

    {
        const std::vector<int> one = {7};
        report("single-stats", same_stats(summarize(one), 1, 7, 7, 7));
    }

    {
        const std::vector<int> mixed = {-5, 10, 0, -2, 10};
        report("mixed-stats", same_stats(summarize(mixed), 5, 13, -5, 10));
    }

    {
        const std::vector<int> large(1000, 1000000);
        report("large-sum", same_stats(summarize(large), 1000, 1000000000LL, 1000000, 1000000));
    }

    {
        std::vector<int> values = {0, 0, 3, 4, 0};
        const std::span<const int> trimmed = trim_zeros(values);
        const bool ok = trimmed.size() == 2 && trimmed[0] == 3 && trimmed[1] == 4 &&
                        trimmed.data() == values.data() + 2;
        report("trim-both-ends", ok);
    }

    {
        std::vector<int> zeros = {0, 0, 0};
        const std::vector<int> empty;
        const std::vector<int> plain = {1, 2};
        const bool ok = trim_zeros(zeros).empty() && trim_zeros(empty).empty() &&
                        trim_zeros(plain).size() == 2 &&
                        trim_zeros(plain).data() == plain.data();
        report("trim-edges", ok);
    }

    {
        std::vector<int> values = {0, 5, 0, 6, 0};
        const std::span<const int> trimmed = trim_zeros(values);
        const bool ok = trimmed.size() == 3 && trimmed[0] == 5 && trimmed[1] == 0 &&
                        trimmed[2] == 6 && trimmed.data() == values.data() + 1;
        report("trim-inner-zero", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
