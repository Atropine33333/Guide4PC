// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
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
        DisjointSet dsu(5);
        bool ok = dsu.components() == 5;
        for (int i = 0; i < 5; ++i) {
            if (dsu.find(i) != i) ok = false;
            for (int j = 0; j < 5; ++j) {
                if (i != j && dsu.connected(i, j)) ok = false;
            }
        }
        report("singleton", ok);
    }

    {
        DisjointSet dsu(5);
        bool ok = dsu.unite(0, 1) && dsu.unite(1, 2);
        ok = ok && dsu.connected(0, 2) && dsu.components() == 3;
        report("chain", ok);
    }

    {
        DisjointSet dsu(5);
        const bool first = dsu.unite(3, 4);
        const bool second = dsu.unite(4, 3);
        report("duplicate", first && !second && dsu.components() == 4);
    }

    {
        DisjointSet dsu(0);
        report("empty", dsu.components() == 0);
    }

    {
        DisjointSet dsu(10);
        bool ok = true;
        for (int i = 0; i + 1 < 10; ++i) ok = ok && dsu.unite(i, i + 1);
        ok = ok && dsu.components() == 1;
        for (int i = 1; i < 10; ++i) ok = ok && dsu.connected(0, i);
        report("all-connected", ok);
    }

    {
        DisjointSet dsu(6);
        dsu.unite(0, 1);
        dsu.unite(2, 3);
        dsu.unite(4, 5);
        const bool ok = dsu.components() == 3 && dsu.connected(0, 1) &&
                        dsu.connected(2, 3) && !dsu.connected(0, 2) &&
                        !dsu.connected(3, 4);
        report("separate", ok);
    }

    {
        DisjointSet dsu(6);
        dsu.unite(0, 1);
        dsu.unite(1, 2);
        dsu.unite(4, 5);
        const int root_a = dsu.find(0);
        const int root_b = dsu.find(1);
        const int root_c = dsu.find(2);
        const int root_d = dsu.find(4);
        const int root_e = dsu.find(5);
        const bool ok = root_a == root_b && root_b == root_c && root_a != root_d &&
                        root_d == root_e && dsu.find(3) == 3;
        report("find-roots", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
