// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <string>
#include <utility>

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
    {
        int value = 5;
        const bool first = describe(value) == "lvalue";
        value = 6;
        report("lvalue", first && describe(value) == "lvalue");
    }

    {
        const int value = 7;
        report("const-lvalue", describe(value) == "const-lvalue");
    }

    {
        int value = 5;
        const bool literal = describe(42) == "rvalue";
        const bool expression = describe(value + 1) == "rvalue";
        report("rvalue", literal && expression);
    }

    {
        int value = 9;
        report("move", describe(std::move(value)) == "rvalue");
    }

    {
        int value = 11;
        describe(value);
        describe(std::move(value));
        report("unchanged", value == 11);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
