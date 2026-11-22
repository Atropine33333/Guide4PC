// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <string>

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
    report("truncate-long", truncate_field("abcdef", 4) == "abcd" &&
                                truncate_field("hello world", 5) == "hello" &&
                                truncate_field("a b c", 3) == "a b" &&
                                truncate_field("abcdef", 1) == "a");

    report("truncate-short", truncate_field("abc", 5) == "abc" &&
                                 truncate_field("abc", 3) == "abc" &&
                                 truncate_field("", 4).empty() &&
                                 truncate_field("abc", 0).empty());

    report("center-even", center_field("ab", 6) == "  ab  " &&
                              center_field("abcd", 8) == "  abcd  " &&
                              center_field("", 4) == "    ");

    report("center-odd", center_field("ab", 5) == " ab  " &&
                             center_field("abc", 6) == " abc  " &&
                             center_field("x", 4) == " x  ");

    report("center-truncate", center_field("abcdef", 4) == "abcd" &&
                                  center_field("abcdef", 0).empty() &&
                                  center_field("abc", 3) == "abc");

    report("empty", truncate_field("", 0).empty() &&
                        center_field("", 0).empty() &&
                        center_field("", 1) == " " &&
                        truncate_field(" ", 1) == " ");

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
