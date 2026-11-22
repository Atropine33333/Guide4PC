// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <limits>
#include <random>

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
        int out = 0;
        const bool ok = add_ints(2, 3, out) && out == 5 &&
                        add_ints(-5, 3, out) && out == -2 &&
                        add_ints(0, 0, out) && out == 0;
        report("add-simple", ok);
    }

    {
        const int low = std::numeric_limits<int>::min();
        const int high = std::numeric_limits<int>::max();
        int out = 0;
        const bool a = add_ints(high, 0, out) && out == high;
        const bool b = add_ints(low, 0, out) && out == low;
        const bool c = !add_ints(high, 1, out) && out == low;
        int untouched = 424242;
        const bool d = !add_ints(high, high, untouched) && untouched == 424242;
        report("add-bounds", a && b && c && d);
    }

    {
        const int low = std::numeric_limits<int>::min();
        const int high = std::numeric_limits<int>::max();
        int out = 0;
        const bool a = sub_ints(5, 3, out) && out == 2;
        const bool b = !sub_ints(low, 1, out) && out == 2;
        int untouched = -7;
        const bool c = !sub_ints(high, -1, untouched) && untouched == -7;
        const bool d = !sub_ints(0, low, untouched) && untouched == -7;
        report("sub-bounds", a && b && c && d);
    }

    {
        int out = 0;
        const bool ok = mul_ints(6, 7, out) && out == 42 &&
                        mul_ints(-6, 7, out) && out == -42 &&
                        mul_ints(0, std::numeric_limits<int>::min(), out) && out == 0 &&
                        mul_ints(std::numeric_limits<int>::max(), 1, out) &&
                        out == std::numeric_limits<int>::max();
        report("mul-simple", ok);
    }

    {
        const int low = std::numeric_limits<int>::min();
        const int high = std::numeric_limits<int>::max();
        int untouched = 99;
        const bool a = !mul_ints(high, 2, untouched) && untouched == 99;
        const bool b = !mul_ints(low, -1, untouched) && untouched == 99;
        const bool c = !mul_ints(46341, 46341, untouched) && untouched == 99;
        const bool d = !mul_ints(low, low, untouched) && untouched == 99;
        report("mul-bounds", a && b && c && d);
    }

    {
        int out = 31337;
        const bool ok = !add_ints(std::numeric_limits<int>::max(), 1, out) &&
                        out == 31337 &&
                        !sub_ints(std::numeric_limits<int>::min(), 1, out) &&
                        out == 31337 &&
                        !mul_ints(100000, 100000, out) && out == 31337;
        report("unchanged", ok);
    }

    {
        std::mt19937 rng(987654321u);
        bool ok = true;
        for (int i = 0; i < 200 && ok; ++i) {
            const int a = static_cast<int>(rng());
            const int b = static_cast<int>(rng());

            const long long sum = static_cast<long long>(a) + b;
            int out = 12345;
            const bool expect_sum = sum >= std::numeric_limits<int>::min() &&
                                    sum <= std::numeric_limits<int>::max();
            if (add_ints(a, b, out) != expect_sum) {
                ok = false;
            } else if (expect_sum && out != static_cast<int>(sum)) {
                ok = false;
            } else if (!expect_sum && out != 12345) {
                ok = false;
            }

            if (!ok) break;
            const long long product = static_cast<long long>(a) * b;
            int product_out = -54321;
            const bool expect_product = product >= std::numeric_limits<int>::min() &&
                                        product <= std::numeric_limits<int>::max();
            if (mul_ints(a, b, product_out) != expect_product) {
                ok = false;
            } else if (expect_product && product_out != static_cast<int>(product)) {
                ok = false;
            } else if (!expect_product && product_out != -54321) {
                ok = false;
            }
        }
        report("random", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
