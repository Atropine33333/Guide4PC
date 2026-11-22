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
    {
        SharedInt a(7);
        report("create", a.value() == 7 && a.use_count() == 1);
    }

    {
        SharedInt a(7);
        SharedInt b(a);
        const bool before = a.use_count() == 2 && b.use_count() == 2 && b.value() == 7;
        b.set_value(9);
        report("copy", before && a.value() == 9 && b.value() == 9);
    }

    {
        SharedInt a(1);
        SharedInt b(2);
        b = a;
        const bool counts = a.use_count() == 2 && b.use_count() == 2;
        a.set_value(5);
        report("assign", counts && b.value() == 5);
    }

    {
        SharedInt a(3);
        {
            SharedInt b(a);
            b.set_value(4);
        }
        report("scope", a.use_count() == 1 && a.value() == 4);
    }

    {
        SharedInt a(6);
        SharedInt& alias = a;
        a = alias;
        report("self", a.use_count() == 1 && a.value() == 6);
    }

    {
        SharedInt a(1);
        SharedInt b(2);
        report("independent", a.use_count() == 1 && b.use_count() == 1 &&
                                  a.value() == 1 && b.value() == 2);
    }

    {
        SharedInt a(8);
        SharedInt b(a);
        SharedInt c(b);
        const bool counts = a.use_count() == 3 && b.use_count() == 3 && c.use_count() == 3;
        {
            SharedInt d(c);
            d.set_value(11);
        }
        report("chain", counts && a.use_count() == 3 && a.value() == 11 &&
                            c.value() == 11);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
