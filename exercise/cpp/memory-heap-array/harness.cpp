// Guide4PC harness appended to the student's code in the same translation unit.
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
        const bool ok = make_zero_array(0) == nullptr &&
                        make_zero_array(-1) == nullptr &&
                        make_zero_array(-100) == nullptr;
        report("non-positive", ok);
    }

    {
        int *values = make_zero_array(10);
        bool ok = values != nullptr;
        for (int i = 0; ok && i < 10; ++i) {
            if (values[i] != 0) ok = false;
        }
        free_array(values);

        int *single = make_zero_array(1);
        ok = ok && single != nullptr && single[0] == 0;
        free_array(single);
        report("zeroed", ok);
    }

    {
        int local[5] = {0, 0, 0, 0, 0};
        fill_sequence(local, 5, 10);
        bool ok = local[0] == 10 && local[4] == 14;
        fill_sequence(local, 5, -3);
        ok = ok && local[0] == -3 && local[4] == 1;
        fill_sequence(local, 0, 99);
        ok = ok && local[0] == -3;
        report("fill", ok);
    }

    {
        int *values = make_zero_array(6);
        fill_sequence(values, 6, 1);
        const bool first = sum_array(values, 6) == 21;
        const bool second = sum_array(values, 0) == 0 &&
                            sum_array(values, -2) == 0 &&
                            sum_array(nullptr, 5) == 0;
        free_array(values);
        report("sum", first && second);
    }

    {
        int *a = make_zero_array(4);
        int *b = make_zero_array(4);
        const bool distinct = a != b;
        fill_sequence(a, 4, 100);
        const bool independent = b[0] == 0 && b[3] == 0 && a[0] == 100;
        free_array(a);
        free_array(b);
        report("independent", distinct && independent);
    }

    {
        int *values = make_zero_array(1000);
        fill_sequence(values, 1000, 1);
        const bool ok = sum_array(values, 1000) == 500500;
        free_array(values);
        report("large", ok);
    }

    {
        free_array(nullptr);
        int *value = new int[3]();
        free_array(value);
        report("free", true);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
