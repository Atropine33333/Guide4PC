// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <functional>
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
        std::function<int()> counter = make_counter(0);
        const bool ok = counter() == 1 && counter() == 2 && counter() == 3;
        report("counter-basic", ok);
    }

    {
        std::function<int()> counter = make_counter(-5);
        const bool ok = counter() == -4 && counter() == -3;
        report("counter-negative", ok);
    }

    {
        std::function<int()> first = make_counter(10);
        std::function<int()> second = make_counter(100);
        const bool ok = first() == 11 && second() == 101 && first() == 12 &&
                        second() == 102;
        report("counter-independent", ok);
    }

    {
        std::function<int(int)> accumulator = make_accumulator(10);
        const bool ok = accumulator(5) == 15 && accumulator(-3) == 12 &&
                        accumulator(0) == 12;
        report("accumulator-basic", ok);
    }

    {
        std::function<int(int)> accumulator = make_accumulator(0);
        const bool ok = accumulator(0) == 0 && accumulator(7) == 7;
        report("accumulator-zero", ok);
    }

    {
        std::function<int()> counter = make_counter(5);
        counter();
        counter();
        std::function<int()> copy = counter;
        const bool ok = copy() == 8 && counter() == 8 && copy() == 9 &&
                        counter() == 9;
        report("copy-independent", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
