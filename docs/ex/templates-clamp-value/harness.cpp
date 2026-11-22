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

}  // namespace

int main() {
    report("middle", clamp_value(5, 1, 10) == 5);

    report("below", clamp_value(-3, 0, 10) == 0);

    report("above", clamp_value(42, 0, 10) == 10);

    {
        const bool ok = clamp_value(10, 0, 10) == 10 && clamp_value(0, 0, 10) == 0;
        report("ends", ok);
    }

    {
        const bool ok = clamp_value(2.5, 0.0, 1.0) == 1.0 &&
                        clamp_value(-0.5, 0.0, 1.0) == 0.0 &&
                        clamp_value(0.25, 0.0, 1.0) == 0.25;
        report("double", ok);
    }

    {
        const bool ok = clamp_value('m', 'a', 'z') == 'm' &&
                        clamp_value('A', 'a', 'z') == 'a' &&
                        clamp_value('~', 'a', 'z') == 'z';
        report("char", ok);
    }

    report("degenerate", clamp_value(7, 3, 3) == 3);

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
