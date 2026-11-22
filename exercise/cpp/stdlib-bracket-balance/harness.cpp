// Guide4PC harness appended to the student's code in the same translation unit.
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
    report("empty", balanced(""));

    {
        const bool ok = balanced("()") && balanced("[]") && balanced("{}") &&
                        balanced("()[]{}");
        report("simple-pairs", ok);
    }

    {
        const bool ok = balanced("{[()()]}") && balanced("([{}])") && balanced("(()[])");
        report("nested", ok);
    }

    {
        const bool ok = !balanced("(]") && !balanced("([)]") && !balanced("{[}]");
        report("mismatch", ok);
    }

    {
        const bool ok = !balanced("())") && !balanced("]") && !balanced("()]");
        report("extra-close", ok);
    }

    {
        const bool ok = !balanced("(((") && !balanced("([") && !balanced("{[]");
        report("unclosed", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
