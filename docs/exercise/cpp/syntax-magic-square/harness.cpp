// Guide4PC harness appended to the student's code in the same translation unit.
#include <algorithm>
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

bool is_magic(int n, const std::vector<int>& grid) {
    if (static_cast<int>(grid.size()) != n * n) {
        return false;
    }
    std::vector<int> seen = grid;
    std::sort(seen.begin(), seen.end());
    for (int i = 0; i < n * n; ++i) {
        if (seen[static_cast<std::size_t>(i)] != i + 1) {
            return false;
        }
    }

    const long long target =
        static_cast<long long>(n) * (static_cast<long long>(n) * n + 1) / 2;
    for (int r = 0; r < n; ++r) {
        long long sum = 0;
        for (int c = 0; c < n; ++c) {
            sum += grid[static_cast<std::size_t>(r) * n + c];
        }
        if (sum != target) return false;
    }
    for (int c = 0; c < n; ++c) {
        long long sum = 0;
        for (int r = 0; r < n; ++r) {
            sum += grid[static_cast<std::size_t>(r) * n + c];
        }
        if (sum != target) return false;
    }
    long long diag_left = 0;
    long long diag_right = 0;
    for (int i = 0; i < n; ++i) {
        diag_left += grid[static_cast<std::size_t>(i) * n + i];
        diag_right += grid[static_cast<std::size_t>(i) * n + (n - 1 - i)];
    }
    return diag_left == target && diag_right == target;
}

}  // namespace

int main() {
    report("one-by-one", magic_square(1) == std::vector<int>{1});
    report("three-by-three", is_magic(3, magic_square(3)));
    report("five-by-five", is_magic(5, magic_square(5)));
    report("nine-by-nine", is_magic(9, magic_square(9)));
    report("fifteen-by-fifteen", is_magic(15, magic_square(15)));

    {
        const bool ok = magic_square(0).empty() && magic_square(-3).empty() &&
                        magic_square(2).empty() && magic_square(4).empty();
        report("rejects-even-and-nonpositive", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
