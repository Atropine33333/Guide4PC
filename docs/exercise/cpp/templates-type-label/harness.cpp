// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
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
    report("int", std::string(TypeLabel<int>::name()) == "int" &&
                      TypeLabel<int>::rank() == 1);

    report("double", std::string(TypeLabel<double>::name()) == "double" &&
                         TypeLabel<double>::rank() == 2);

    report("bool", std::string(TypeLabel<bool>::name()) == "bool" &&
                       TypeLabel<bool>::rank() == 3);

    {
        const bool ok = std::string(TypeLabel<int*>::name()) == "pointer" &&
                        TypeLabel<int*>::rank() == 4 &&
                        std::string(TypeLabel<double*>::name()) == "pointer" &&
                        TypeLabel<double*>::rank() == 4;
        report("pointer", ok);
    }

    {
        const bool ok = std::string(TypeLabel<const char*>::name()) == "pointer" &&
                        TypeLabel<const char*>::rank() == 4;
        report("const-pointer", ok);
    }

    {
        const bool ok = std::string(TypeLabel<long>::name()) == "other" &&
                        TypeLabel<long>::rank() == 0 &&
                        std::string(TypeLabel<unsigned>::name()) == "other";
        report("fallback", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
