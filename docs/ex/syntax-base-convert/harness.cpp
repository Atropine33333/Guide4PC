// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <climits>
#include <iostream>
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

}  // namespace

int main() {
    report("zero", to_base(0, 2) == "0" && to_base(0, 36) == "0" &&
                       to_base(0, 10) == "0");

    report("bases", to_base(255, 16) == "FF" && to_base(5, 2) == "101" &&
                        to_base(35, 36) == "Z" && to_base(36, 36) == "10" &&
                        to_base(100, 3) == "10201");

    report("negative", to_base(-5, 2) == "-101" &&
                           to_base(-255, 16) == "-FF" &&
                           to_base(-1, 10) == "-1");

    report("limits", to_base(LLONG_MAX, 16) == "7FFFFFFFFFFFFFFF" &&
                         to_base(LLONG_MIN, 16) == "-8000000000000000" &&
                         to_base(LLONG_MIN, 10) == "-9223372036854775808");

    report("invalid-base", to_base(5, 1).empty() && to_base(5, 37).empty());

    {
        long long out = -1;
        const bool a = parse_base("101", 2, out) && out == 5;
        const bool b = parse_base("-ff", 16, out) && out == -255;
        const bool c = parse_base("+7", 10, out) && out == 7;
        const bool d = parse_base("Z", 36, out) && out == 35;
        report("parse-basic", a && b && c && d);
    }

    {
        long long out = 123;
        const bool a = !parse_base("12", 2, out) && out == 123;
        const bool b = !parse_base("", 10, out);
        const bool c = !parse_base("-", 10, out);
        const bool d = !parse_base("+", 10, out);
        const bool e = !parse_base("1 2", 10, out);
        const bool f = !parse_base("0x10", 16, out);
        const bool g = !parse_base("G", 16, out);
        report("parse-invalid", a && b && c && d && e && f && g);
    }

    {
        long long out = 0;
        const bool a = parse_base("9223372036854775807", 10, out) && out == LLONG_MAX;
        const bool b = !parse_base("9223372036854775808", 10, out);
        const bool c = parse_base("-9223372036854775808", 10, out) && out == LLONG_MIN;
        const bool d = !parse_base("-9223372036854775809", 10, out);
        const bool e = !parse_base("1000000000000000000000", 10, out);
        report("parse-limits", a && b && c && d && e);
    }

    {
        const long long values[] = {0, 1, -1, 12345, -987654321, LLONG_MIN, LLONG_MAX};
        bool ok = true;
        for (long long value : values) {
            for (int base = 2; base <= 36; ++base) {
                long long back = 0;
                if (!parse_base(to_base(value, base), base, back) || back != value) {
                    ok = false;
                }
            }
        }
        report("round-trip", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
