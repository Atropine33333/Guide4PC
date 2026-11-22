// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <string>
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
    {
        const std::string empty;
        const bool ok = common_prefix(empty, empty) == 0 &&
                        common_prefix(empty, "abc") == 0 &&
                        common_prefix("abc", empty) == 0;
        report("empty-input", ok);
    }

    {
        const bool ok = common_prefix("abc", "abc") == 3 &&
                        common_prefix("abc", "abd") == 2 &&
                        common_prefix("abc", "ab") == 2 &&
                        common_prefix("ab", "abc") == 2;
        report("prefix-basic", ok);
    }

    {
        const bool ok = common_prefix("Abc", "abc") == 0 &&
                        common_prefix("ab", "aBc") == 1 &&
                        common_prefix("hello", "help") == 3 &&
                        common_prefix("x", "y") == 0;
        report("prefix-case", ok);
    }

    {
        const std::vector<int> values = {1, 2, 3, 2, 2, 1};
        const std::vector<int> none;
        const bool ok = last_index_of(values, 1) == 5 &&
                        last_index_of(values, 2) == 4 &&
                        last_index_of(values, 3) == 2 &&
                        last_index_of(values, 9) == -1 &&
                        last_index_of(none, 0) == -1;
        report("last-index", ok);
    }

    {
        const bool ok = is_rotation("abcde", "cdeab") &&
                        is_rotation("abcde", "eabcd") &&
                        is_rotation("a", "a") && is_rotation("", "") &&
                        !is_rotation("abcde", "abced");
        report("rotation-basic", ok);
    }

    {
        const bool ok = !is_rotation("ab", "aba") &&
                        !is_rotation("abc", "abcd") &&
                        !is_rotation("aaaa", "aa") &&
                        !is_rotation("", "x") && is_rotation("rotation", "tionrota");
        report("rotation-lengths", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
