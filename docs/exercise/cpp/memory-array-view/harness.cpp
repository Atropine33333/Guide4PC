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
        IntView view;
        const bool ok = view.empty() && view.size() == 0 && view.data() == nullptr &&
                        view.slice(0, 1).empty();
        report("default", ok);
    }

    {
        int values[5] = {1, 2, 3, 4, 5};
        IntView view(values, 5);
        const bool ok = !view.empty() && view.size() == 5 && view.data() == values &&
                        view[0] == 1 && view[4] == 5;
        report("construct", ok);
    }

    {
        int values[5] = {1, 2, 3, 4, 5};
        IntView view(values, 5);
        view[0] = 10;
        view[4] = 50;
        const bool first = values[0] == 10 && values[4] == 50;
        const IntView read_only(values, 5);
        const bool second = read_only[0] == 10 && read_only[4] == 50;
        report("mutate", first && second);
    }

    {
        int values[6] = {1, 2, 3, 4, 5, 6};
        IntView view(values, 6);
        IntView middle = view.slice(1, 3);
        bool ok = middle.size() == 3 && middle[0] == 2 && middle[1] == 3 &&
                  middle[2] == 4 && middle.data() == values + 1;
        middle[1] = 30;
        ok = ok && values[2] == 30 && view[2] == 30;
        report("slice", ok);
    }

    {
        int values[4] = {1, 2, 3, 4};
        IntView view(values, 4);
        const bool ok = view.slice(-1, 2).empty() &&
                        view.slice(0, -1).empty() &&
                        view.slice(4, 1).empty() &&
                        view.slice(2, 3).empty() &&
                        view.slice(0, 0).empty() &&
                        view.slice(0, 4).size() == 4 &&
                        view.slice(2, 2).size() == 2;
        report("invalid-slices", ok);
    }

    {
        int values[3] = {7, 8, 9};
        IntView null_view(nullptr, 3);
        IntView zero_view(values, 0);
        IntView negative_view(values, -2);
        const bool ok = null_view.empty() && zero_view.empty() && negative_view.empty() &&
                        null_view.data() == nullptr;
        report("empty-views", ok);
    }

    {
        int values[5] = {1, 2, 3, 4, 5};
        IntView view(values, 5);
        IntView nested = view.slice(1, 4).slice(1, 2);
        const bool ok = nested.size() == 2 && nested[0] == 3 && nested[1] == 4 &&
                        nested.data() == values + 2;
        report("nested", ok);
    }

    {
        int values[3] = {1, 2, 3};
        const IntView view(values, 3);
        const bool ok = view[0] == 1 && view.size() == 3 && !view.empty() &&
                        view.slice(1, 2).size() == 2;
        report("const", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
