// Guide4PC harness: compiled after the student's code in the same translation unit.
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

bool parses(const std::string& text, int year, int month, int day) {
    Date out{-1, -1, -1};
    return parse_iso_date(text, out) && out.year == year && out.month == month &&
           out.day == day;
}

bool rejects(const std::string& text) {
    Date out{-1, -2, -3};
    const bool ok = !parse_iso_date(text, out);
    return ok && out.year == -1 && out.month == -2 && out.day == -3;
}

}  // namespace

int main() {
    report("valid", parses("2024-02-09", 2024, 2, 9) &&
                        parses("1999-12-31", 1999, 12, 31) &&
                        parses("2000-01-01", 2000, 1, 1) &&
                        parses("0001-01-01", 1, 1, 1) &&
                        parses("9999-12-31", 9999, 12, 31));

    report("leap", parses("2024-02-29", 2024, 2, 29) &&
                       parses("2000-02-29", 2000, 2, 29) &&
                       rejects("1900-02-29") && rejects("2023-02-29") &&
                       rejects("2100-02-29"));

    report("month-length", parses("2023-01-31", 2023, 1, 31) &&
                               parses("2023-04-30", 2023, 4, 30) &&
                               parses("2023-06-30", 2023, 6, 30) &&
                               parses("2023-09-30", 2023, 9, 30) &&
                               parses("2023-11-30", 2023, 11, 30) &&
                               rejects("2023-04-31") && rejects("2023-06-31") &&
                               rejects("2023-09-31") && rejects("2023-11-31"));

    report("reject-shape", rejects("2024-2-09") && rejects("2024-02-9") &&
                               rejects("2024/02/09") && rejects("2024-02-09 ") &&
                               rejects(" 2024-02-09") && rejects("2024-02-09x") &&
                               rejects("2024-02-0a") && rejects("") &&
                               rejects("2024-02-09-01"));

    report("reject-values", rejects("0000-01-01") && rejects("2024-00-10") &&
                                rejects("2024-13-10") && rejects("2024-01-00") &&
                                rejects("2024-01-32") && rejects("2023-02-29") &&
                                rejects("2024-12-32"));

    report("format", format_short_date({2024, 2, 9}) == "09/02/2024" &&
                         format_short_date({2000, 12, 31}) == "31/12/2000" &&
                         format_short_date({1, 1, 1}) == "01/01/0001" &&
                         format_short_date({9999, 10, 5}) == "05/10/9999");

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
