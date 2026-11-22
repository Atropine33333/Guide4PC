// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

Entry make(const char *name, int score, int id) { return Entry(name, score, id); }

}  // namespace

int main() {
    report("uses-mixin", std::is_base_of_v<Comparable<Entry>, Entry>);

    {
        const Entry a = make("A", 90, 1);
        const Entry b = make("B", 80, 2);
        const Entry copy = a;
        const bool ok = (a != b) && !(a != copy) && (a != copy) == false;
        report("neq", ok);
    }

    {
        const Entry a = make("A", 90, 1);
        const Entry b = make("B", 95, 2);
        const bool ok = (a > b) && !(b > a) && !(a > make("C", 90, 5));
        report("gt", ok);
    }

    {
        const Entry a = make("A", 90, 1);
        const Entry b = make("B", 95, 2);
        const bool ok = (b <= a) && (b <= b) && (a >= b) && (a >= a) && !(a <= b) &&
                        !(b >= a);
        report("le-ge", ok);
    }

    {
        std::vector<Entry> entries = {make("A", 90, 1), make("C", 75, 3),
                                      make("D", 90, 0), make("B", 90, 2)};
        std::sort(entries.begin(), entries.end());
        const bool ok = entries[0].id == 0 && entries[1].id == 1 && entries[2].id == 2 &&
                        entries[3].id == 3;
        report("sort", ok);
    }

    {
        std::vector<Entry> entries = {make("A", 90, 1), make("B", 95, 2),
                                      make("C", 95, 0)};
        const auto best = std::max_element(entries.begin(), entries.end());
        const bool ok = best != entries.end() && best->name == "A";
        report("max-element", ok);
    }

    {
        const Entry a = make("A", 90, 1);
        const Entry b = make("A", 90, 2);
        const Entry name_differs = make("Z", 90, 1);
        const bool ok = (a != b) && (a < b) && !(b < a) && (a != name_differs) &&
                        !(a < name_differs) && !(name_differs < a) && !(a == b);
        report("equal-prefix", ok);
    }

    {
        const Entry items[] = {make("A", 90, 1), make("B", 95, 2), make("C", 95, 0)};
        bool ok = true;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                const Entry& lhs = items[i];
                const Entry& rhs = items[j];
                if ((lhs < rhs) != (rhs > lhs)) ok = false;
                if ((lhs <= rhs) != !(rhs < lhs)) ok = false;
                if ((lhs >= rhs) != !(lhs < rhs)) ok = false;
                if ((lhs != rhs) != !(lhs == rhs)) ok = false;
            }
        }
        report("consistent", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
