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

}  // namespace

int main() {
    {
        const bool ok = luhn_valid("79927398713") && luhn_valid("4111111111111111") &&
                        luhn_valid("5555555555554444") && luhn_valid("4012888888881881");
        report("valid", ok);
    }

    {
        const bool ok = !luhn_valid("79927398710") && !luhn_valid("1234567812345678") &&
                        !luhn_valid("79927398712");
        report("invalid", ok);
    }

    {
        const bool ok = luhn_check_digit("7992739871") == '3' &&
                        luhn_check_digit("12345") == '5' &&
                        luhn_check_digit("0") == '0' &&
                        luhn_check_digit("999999999999999") == '5';
        report("check-digit", ok);
    }

    {
        const bool ok = luhn_valid("4111 1111 1111 1111") &&
                        luhn_valid("7992-7398-713") &&
                        luhn_valid("  4539 5787 6362 1486  ") &&
                        luhn_check_digit("4111 1111 1111 111") == '1';
        report("ignore-separators", ok);
    }

    {
        const bool ok = !luhn_valid("") && !luhn_valid("   ") && !luhn_valid("5") &&
                        luhn_valid("0") && luhn_valid("00") &&
                        luhn_check_digit("") == '0' && luhn_check_digit("   ") == '0';
        report("short", ok);
    }

    {
        const std::string payload(200, '9');
        const char digit = luhn_check_digit(payload);
        const bool ok = digit >= '0' && digit <= '9' && luhn_valid(payload + digit);
        report("long", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
