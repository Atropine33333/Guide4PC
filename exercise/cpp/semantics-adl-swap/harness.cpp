// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <iostream>
#include <utility>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool same(const gadget::Widget& w, int id, int value) {
    return w.id == id && w.value == value;
}

}  // namespace

int main() {
    {
        gadget::reset_swap_calls();
        gadget::Widget x{1, 10};
        gadget::Widget y{2, 20};
        {
            using std::swap;
            swap(x, y);
        }
        const bool ok = same(x, 2, 20) && same(y, 1, 10) && gadget::swap_calls() == 1;
        report("adl-swap", ok);
    }

    {
        gadget::reset_swap_calls();
        gadget::Widget x{3, 30};
        gadget::Widget y{4, 40};
        std::swap(x, y);
        const bool ok = same(x, 4, 40) && same(y, 3, 30) && gadget::swap_calls() == 0;
        report("std-swap", ok);
    }

    {
        gadget::reset_swap_calls();
        int a = 5;
        int b = 6;
        {
            using std::swap;
            swap(a, b);
        }
        report("builtin-swap", a == 6 && b == 5 && gadget::swap_calls() == 0);
    }

    {
        gadget::Widget x{7, 70};
        gadget::Widget y{8, 80};
        {
            using std::swap;
            swap(x, y);
        }
        const bool first = gadget::swap_calls() == 1;
        gadget::reset_swap_calls();
        const bool second = gadget::swap_calls() == 0;
        {
            using std::swap;
            swap(x, y);
        }
        report("counter-reset", first && second && gadget::swap_calls() == 1);
    }

    {
        gadget::reset_swap_calls();
        gadget::Widget x{9, 90};
        gadget::Widget y{10, 100};
        gadget::swap(x, y);
        const bool ok = same(x, 10, 100) && same(y, 9, 90) && gadget::swap_calls() == 1;
        report("qualified-swap", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
