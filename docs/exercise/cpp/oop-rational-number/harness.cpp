// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool same(const Rational& value, int numerator, int denominator) {
    return value.numerator() == numerator && value.denominator() == denominator;
}

}  // namespace

int main() {
    {
        const bool ok = same(Rational(2, 4), 1, 2) && same(Rational(-2, 4), -1, 2) &&
                        same(Rational(2, -4), -1, 2) && same(Rational(0, 5), 0, 1) &&
                        same(Rational(-4, -6), 2, 3);
        report("normalize", ok);
    }

    {
        const bool ok = same(Rational(1, 3) + Rational(1, 6), 1, 2) &&
                        same(Rational(1, 2) + Rational(1, 2), 1, 1);
        report("add", ok);
    }

    {
        const bool ok = same(Rational(3, 4) - Rational(1, 6), 7, 12) &&
                        same(Rational(1, 2) - Rational(1, 2), 0, 1);
        report("sub", ok);
    }

    {
        const bool ok = same(Rational(2, 3) * Rational(3, 4), 1, 2) &&
                        same(Rational(-2, 3) * Rational(3, 2), -1, 1);
        report("mul", ok);
    }

    {
        const bool ok = same(Rational(1, 2) / Rational(3, 4), 2, 3) &&
                        same(Rational(-1, 2) / Rational(1, 4), -2, 1);
        report("div", ok);
    }

    {
        const bool ok = Rational(1, 3) < Rational(1, 2) &&
                        !(Rational(1, 2) < Rational(1, 3)) &&
                        Rational(2, 4) == Rational(1, 2) &&
                        Rational(1, 3) == Rational(2, 6);
        report("compare", ok);
    }

    {
        const bool ok = same(Rational(-1, 2) + Rational(1, 3), -1, 6) &&
                        same(Rational(1, 3) - Rational(1, 2), -1, 6);
        report("negative", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
