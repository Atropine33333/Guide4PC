// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <iostream>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

}  // namespace

int main() {
    {
        Polynomial zero;
        report("zero", zero.degree() == -1 && zero.coefficient(0) == 0 &&
                           zero.coefficient(7) == 0 && zero.evaluate(99) == 0 &&
                           zero == Polynomial({0, 0, 0}));
    }

    {
        Polynomial value({1, 2, 0, 0});
        report("normalize", value.degree() == 1 && value.coefficient(0) == 1 &&
                                value.coefficient(1) == 2 &&
                                value.coefficient(2) == 0 &&
                                value.coefficient(-1) == 0 &&
                                value.coefficient(10) == 0);
    }

    {
        Polynomial value({1, 2, 3});
        report("evaluate", value.evaluate(2) == 17 && value.evaluate(0) == 1 &&
                               value.evaluate(-1) == 2 &&
                               value.evaluate(1) == 6);
    }

    {
        Polynomial left({1, 2});
        Polynomial right({0, 3, 4});
        const Polynomial sum = left + right;
        const Polynomial cancel = Polynomial({1, 1}) + Polynomial({-1, 1});
        report("add", sum.degree() == 2 && sum.coefficient(0) == 1 &&
                          sum.coefficient(1) == 5 &&
                          sum.coefficient(2) == 4 && cancel.degree() == 1 &&
                          cancel.coefficient(0) == 0 &&
                          cancel.coefficient(1) == 2);
    }

    {
        const Polynomial product = Polynomial({1, 1}) * Polynomial({1, -1});
        const Polynomial zero = Polynomial({0}) * Polynomial({5, 6});
        report("multiply", product.degree() == 2 && product.coefficient(0) == 1 &&
                               product.coefficient(1) == 0 &&
                               product.coefficient(2) == -1 &&
                               zero.degree() == -1 &&
                               (Polynomial({2, 3}) * Polynomial({4})).degree() ==
                                   1);
    }

    {
        Polynomial left({1, 2, 0});
        Polynomial right({1, 2});
        report("equality", left == right && !(left == Polynomial({2, 1})) &&
                               left != Polynomial({1, 3}) &&
                               Polynomial() == Polynomial({0}));
    }

    {
        Polynomial power({1, 1});
        Polynomial result({1});
        for (int i = 0; i < 10; ++i) result = result * power;
        report("power", result.degree() == 10 && result.coefficient(0) == 1 &&
                            result.coefficient(5) == 252 &&
                            result.coefficient(10) == 1 &&
                            result.evaluate(1) == 1024);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
