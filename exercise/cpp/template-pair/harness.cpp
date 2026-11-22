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
        Pair<int> pair(1, 2);
        pair.second() = 20;
        const bool ok = pair.first() == 1 && pair.second() == 20;
        report("int-pair", ok);
    }

    {
        Pair<std::string> pair("left", "right");
        const Pair<std::string> swapped = pair.swapped();
        const bool ok = swapped.first() == "right" && swapped.second() == "left" &&
                        pair.first() == "left" && pair.second() == "right";
        report("string-pair", ok);
    }

    {
        Pair<int> first(3, 4);
        Pair<int> same(3, 4);
        Pair<int> different(4, 3);
        const bool ok = first == same && !(first == different);
        report("equality", ok);
    }

    {
        Pair<Pair<int>> nested(Pair<int>(1, 2), Pair<int>(3, 4));
        const Pair<Pair<int>> swapped = nested.swapped();
        const bool ok = swapped.first() == Pair<int>(3, 4) &&
                        swapped.second() == Pair<int>(1, 2) &&
                        nested.first() == Pair<int>(1, 2);
        report("nested-pair", ok);
    }

    {
        const Pair<int> pair(7, 8);
        const int first = pair.first();
        const int second = pair.second();
        const bool ok = first == 7 && second == 8 && pair.swapped().first() == 8;
        report("const-access", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
