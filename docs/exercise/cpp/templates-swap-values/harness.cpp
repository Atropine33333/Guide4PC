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
    {
        int a = 1;
        int b = 2;
        swap_values(a, b);
        report("ints", a == 2 && b == 1 && min_value(3, 5) == 3 &&
                       min_value(5, 3) == 3);
    }

    {
        double a = 2.5;
        double b = -1.5;
        swap_values(a, b);
        report("doubles", a == -1.5 && b == 2.5 && min_value(-1.5, 2.5) == -1.5);
    }

    {
        std::string a = "apple";
        std::string b = "banana";
        swap_values(a, b);
        report("strings", a == "banana" && b == "apple" &&
                          min_value(std::string("pear"), std::string("apple")) == "apple");
    }

    {
        int value = 5;
        swap_values(value, value);
        report("self-swap", value == 5 && min_value(value, value) == 5);
    }

    {
        const bool ok = min_value(7, 7) == 7 && min_value('z', 'a') == 'a' &&
                        min_value(2.5, 2.5) == 2.5;
        report("equal", ok);
    }

    {
        char a = 'x';
        char b = 'y';
        swap_values(a, b);
        report("chars", a == 'y' && b == 'x');
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
