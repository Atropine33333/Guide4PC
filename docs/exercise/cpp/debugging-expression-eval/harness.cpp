// Guide4PC harness appended to the student's code in the same translation unit.
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

bool check(const std::string& text, int expected) {
    return eval_expression(text) == expected;
}

}  // namespace

int main() {
    {
        report("precedence", check("1+2*3", 7) && check("(1+2)*3", 9) &&
                                 check("8-2*3", 2));
    }

    {
        report("left-associative", check("10-3-2", 5) &&
                                       check("2*3*4", 24) &&
                                       check("20-5-3-2", 10));
    }

    {
        report("unary-minus", check("-5", -5) && check("2*-3", -6) &&
                                  check("-(2+3)", -5) && check("--4", 4) &&
                                  check("-(-3)", 3));
    }

    {
        report("unary-with-spaces", check(" - 5 ", -5) &&
                                        check("2 * - 3", -6) &&
                                        check("- ( 2 + 3 )", -5));
    }

    {
        report("nested", check("((2+3)*(4-1))", 15) &&
                              check("-(1+(2*3))", -7) &&
                              check("2*(3+(4-1)*2)", 18));
    }

    {
        report("zero", check("0", 0) && check("0*5", 0) &&
                           check("-(0)", 0) && check("0-0", 0));
    }

    {
        report("larger", check("100*100+250", 10250) &&
                             check("12345", 12345) &&
                             check("(10+20)*(30-20)", 300));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
