// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <sstream>
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

std::string h_print(const Money &money) {
    std::ostringstream out;
    out << money;
    return out.str();
}

bool h_parse(const std::string &text, Money &money) {
    std::istringstream in(text);
    in >> money;
    return !in.fail();
}

}  // namespace

int main() {
    {
        const bool ok = h_print(Money(1234)) == "$12.34" && h_print(Money(5)) == "$0.05" &&
                        h_print(Money(0)) == "$0.00" && h_print(Money(100000)) == "$1000.00";
        report("print-basic", ok);
    }

    {
        const bool ok = h_print(Money(-1234)) == "-$12.34" && h_print(Money(-5)) == "-$0.05";
        report("print-negative", ok);
    }

    {
        Money seven;
        Money half;
        Money big;
        const bool ok = h_parse("12.34", seven) && seven.cents() == 1234 &&
                        h_parse("0.5", half) && half.cents() == 50 &&
                        h_parse("7", big) && big.cents() == 700;
        report("parse-basic", ok);
    }

    {
        Money negative;
        Money dollar;
        const bool ok = h_parse("-$0.05", negative) && negative.cents() == -5 &&
                        h_parse("$3.25", dollar) && dollar.cents() == 325;
        report("parse-sign-dollar", ok);
    }

    {
        bool ok = true;
        const long long values[] = {0, 1, 99, 100, 123456789, -987654321};
        for (long long value : values) {
            Money money(value);
            Money parsed;
            std::ostringstream out;
            out << money;
            if (!h_parse(out.str(), parsed) || !(parsed == money)) ok = false;
        }
        report("roundtrip", ok);
    }

    {
        Money money(777);
        std::istringstream in("abc");
        in >> money;
        Money unchanged(777);
        Money second(888);
        std::istringstream bad("1.234");
        bad >> second;
        const bool ok = in.fail() && money == unchanged && bad.fail() && second.cents() == 888;
        report("fail", ok);
    }

    {
        const Money a(1000);
        const Money b(234);
        const bool ok = (a + b).cents() == 1234 && (a - b).cents() == 766 &&
                        (b * 3).cents() == 702 && b < a && a == (b + Money(766)) &&
                        !(a < b);
        report("arithmetic", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
