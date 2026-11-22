// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <set>
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
    const std::set<int> values = {-7, -2, 0, 1, 3, 4, 8, 15, 16, 23, 42};

    report("inclusive", window_values(values, 3, 8) == std::vector<int>({3, 4, 8}) &&
                            window_values(values, 4, 8) == std::vector<int>({4, 8}) &&
                            window_values(values, 3, 4) == std::vector<int>({3, 4}) &&
                            window_count(values, 3, 8) == 3 && window_count(values, 4, 8) == 2 &&
                            window_count(values, 3, 4) == 2);

    report("case-empty", window_values(std::set<int>{}, 0, 100).empty() &&
                             window_count(std::set<int>{}, 0, 100) == 0);

    report("outside", window_values(values, 9, 14).empty() &&
                          window_values(values, -6, -3).empty() &&
                          window_values(values, 43, 100).empty() &&
                          window_count(values, 9, 14) == 0 &&
                          window_count(values, -6, -3) == 0 &&
                          window_count(values, 43, 100) == 0);

    report("reversed", window_values(values, 8, 3).empty() &&
                           window_values(values, 10, -10).empty() &&
                           window_count(values, 8, 3) == 0 &&
                           window_count(values, 10, -10) == 0);

    const std::set<int> single = {5};
    report("single", window_values(single, 5, 5) == std::vector<int>({5}) &&
                         window_values(single, 4, 6) == std::vector<int>({5}) &&
                         window_count(single, 5, 5) == 1 && window_count(single, 6, 9) == 0);

    const std::set<int> sparse = {-1000000, -1, 0, 1, 1000000};
    report("sparse", window_values(sparse, -1000000, 0) ==
                             std::vector<int>({-1000000, -1, 0}) &&
                         window_values(sparse, 0, 1000000) ==
                             std::vector<int>({0, 1, 1000000}) &&
                         window_count(sparse, -1000000, 1000000) == 5 &&
                         window_count(sparse, -999999, 999999) == 3);

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
