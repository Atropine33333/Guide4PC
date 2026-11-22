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
        FixedArray<int, 4> array;
        const FixedArray<int, 4>& view = array;
        bool ok = array.size() == 4;
        for (int i = 0; i < 4; ++i) ok = ok && view[i] == 0;
        report("int-default", ok);
    }

    {
        FixedArray<int, 3> array;
        array.fill(7);
        report("fill", array[0] == 7 && array[1] == 7 && array[2] == 7);
    }

    {
        FixedArray<int, 3> array;
        array[1] = 9;
        const FixedArray<int, 3>& view = array;
        report("write", view[1] == 9 && view[0] == 0 && view[2] == 0);
    }

    {
        FixedArray<double, 3> array;
        const FixedArray<double, 3>& view = array;
        report("double", view[0] == 0.0 && view[1] == 0.0 && view[2] == 0.0);
    }

    {
        FixedArray<int, 3> array;
        array[1] = 5;
        int value = 42;
        const bool valid = array.try_get(1, value) && value == 5;
        value = 42;
        const bool low = !array.try_get(-1, value) && value == 42;
        const bool high = !array.try_get(3, value) && value == 42;
        report("bounds", valid && low && high);
    }

    {
        FixedArray<int, 2> array;
        array[0] = 11;
        const FixedArray<int, 2>& view = array;
        report("pointer", array.data() == &array[0] && view.data() == &view[0] &&
                              array.data()[0] == 11);
    }

    {
        FixedArray<char, 2> array;
        array.fill('x');
        report("char", array[0] == 'x' && array[1] == 'x');
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
