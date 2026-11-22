// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <cmath>
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

bool near(double a, double b) {
    return std::fabs(a - b) < 1e-9;
}

}  // namespace

int main() {
    {
        const int values[] = {2, 4, 6};
        report("ints", near(average(values), 4.0) && max_value(values) == 6);
    }

    {
        const double values[] = {1.5, -2.0, 3.5};
        report("doubles", near(average(values), 1.0) && max_value(values) == 3.5);
    }

    {
        const long long values[] = {10000000000LL, 20000000000LL};
        report("longlong", near(average(values), 15000000000.0) &&
                           max_value(values) == 20000000000LL);
    }

    {
        const int values[] = {42};
        report("single", near(average(values), 42.0) && max_value(values) == 42);
    }

    {
        const char values[] = {'a', 'z', 'm'};
        report("chars", near(average(values), 328.0 / 3.0) && max_value(values) == 'z');
    }

    {
        const std::string values[] = {"pear", "apple", "zebra"};
        report("strings", max_value(values) == "zebra");
    }

    {
        const int values[] = {-5, -1, -3};
        report("negatives", near(average(values), -3.0) && max_value(values) == -1);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
