// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <optional>
#include <string>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool parsed_is(const std::string &text, int expected) {
    const std::optional<int> value = parse_int(text);
    return value.has_value() && *value == expected;
}

bool parsed_none(const std::string &text) {
    return !parse_int(text).has_value();
}

}  // namespace

int main() {
    report("valid", parsed_is("0", 0) && parsed_is("42", 42) &&
                        parsed_is("-7", -7) && parsed_is("000123", 123));

    report("whitespace", parsed_none("") && parsed_none("-") &&
                             parsed_none(" 1") && parsed_none("1 ") &&
                             parsed_none("1 2") && parsed_none("1\n"));

    report("syntax", parsed_none("+1") && parsed_none("--1") &&
                         parsed_none("1.5") && parsed_none("12a") &&
                         parsed_none("-+3") && parsed_none("0x10"));

    report("range", parsed_is("2147483647", 2147483647) &&
                        parsed_is("-2147483648", -2147483647 - 1) &&
                        parsed_none("2147483648") &&
                        parsed_none("-2147483649") &&
                        parsed_none("99999999999999999999"));

    report("negative-zero", parsed_is("-0", 0) && parsed_is("-000", 0) &&
                                parsed_is("0000", 0));

    report("parse-or", parse_or("17", -1) == 17 && parse_or("x", -1) == -1 &&
                           parse_or("", 5) == 5 &&
                           parse_or("2147483648", 9) == 9 &&
                           parse_or("-2147483648", 0) == -2147483647 - 1);

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
