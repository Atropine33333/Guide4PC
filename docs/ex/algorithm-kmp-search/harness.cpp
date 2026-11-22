// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
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
    report("basic", find_all("abracadabra", "abra") ==
                        std::vector<std::size_t>({0, 7}) &&
                        find_all("hello world", "world") ==
                            std::vector<std::size_t>({6}) &&
                        find_all("abcabcabc", "abc") ==
                            std::vector<std::size_t>({0, 3, 6}));

    report("overlap", find_all("aaaaa", "aaa") ==
                          std::vector<std::size_t>({0, 1, 2}) &&
                          find_all("ababababa", "aba") ==
                              std::vector<std::size_t>({0, 2, 4, 6}) &&
                          find_all("aaaa", "aa") ==
                              std::vector<std::size_t>({0, 1, 2}));

    report("empty", find_all("abc", "").empty() && find_all("", "").empty() &&
                        find_all("abc", "abcd").empty() &&
                        find_all("", "a").empty());

    report("edges", find_all("abc", "abc") == std::vector<std::size_t>({0}) &&
                        find_all("xabc", "abc") == std::vector<std::size_t>({1}) &&
                        find_all("abcx", "abc") == std::vector<std::size_t>({0}) &&
                        find_all("xabcx", "abc") == std::vector<std::size_t>({1}));

    report("repeated", find_all("aaaaaaaaaa", "a") ==
                           std::vector<std::size_t>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}) &&
                           find_all("abababab", "abab") ==
                               std::vector<std::size_t>({0, 2, 4}) &&
                           find_all("aaaa", "b").empty());

    {
        std::string text;
        for (int i = 0; i < 20000; ++i) text.push_back('a');
        std::string pattern(100, 'a');
        pattern.push_back('b');
        const std::vector<std::size_t> none = find_all(text, pattern);

        std::string haystack;
        for (int i = 0; i < 5000; ++i) haystack.push_back('a' + (i % 26));
        const std::vector<std::size_t> many = find_all(haystack, "abc");
        bool many_ok = true;
        std::size_t expected = 0;
        for (std::size_t i = 0; i + 3 <= haystack.size(); ++i) {
            if (haystack.compare(i, 3, "abc") == 0) {
                if (expected >= many.size() || many[expected] != i) {
                    many_ok = false;
                    break;
                }
                ++expected;
            }
        }
        if (expected != many.size()) many_ok = false;
        report("long", none.empty() && many_ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
