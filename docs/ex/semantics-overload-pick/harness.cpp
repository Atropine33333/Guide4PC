// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <cstring>
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

bool is(const char *actual, const char *expected) {
    return std::strcmp(actual, expected) == 0;
}

}  // namespace

int main() {
    report("int-exact", is(pick(42), "int"));

    report("long-exact", is(pick(42L), "long"));

    report("double-exact", is(pick(4.5), "double"));

    {
        const short small = 7;
        const char letter = 'x';
        const bool flag = true;
        const bool ok = is(pick(small), "int") && is(pick(letter), "int") &&
                        is(pick(flag), "int");
        report("integral-promotions", ok);
    }

    report("float-promotion", is(pick(2.5f), "double"));

    {
        const bool ok = is(pick(0), "int") && is(pick(0L), "long") && is(pick(0.0), "double");
        report("literals", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
