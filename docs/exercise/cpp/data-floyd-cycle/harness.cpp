// Guide4PC harness appended to the student's code in the same translation unit.
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

bool same(const CycleInfo& info, bool has_cycle, int entry, int length) {
    return info.has_cycle == has_cycle && info.entry == entry &&
           info.length == length;
}

}  // namespace

int main() {
    {
        const std::vector<int> next = {1, 2, -1};
        report("no-cycle", same(analyze(next, 0), false, -1, 0) &&
                                same(analyze(next, 1), false, -1, 0) &&
                                same(analyze(next, 2), false, -1, 0));
    }

    {
        const std::vector<int> next = {0};
        report("self-loop", same(analyze(next, 0), true, 0, 1));
    }

    {
        const std::vector<int> next = {1, 0};
        const bool ok = same(analyze(next, 0), true, 0, 2) &&
                        same(analyze(next, 1), true, 1, 2);
        report("two-cycle", ok);
    }

    {
        const std::vector<int> next = {1, 2, 3, 1};
        report("tail-into-cycle", same(analyze(next, 0), true, 1, 3) &&
                                      same(analyze(next, 2), true, 2, 3));
    }

    {
        const std::vector<int> next = {1, 2, 1, 2, -1};
        const bool ok = same(analyze(next, 3), true, 2, 2) &&
                        same(analyze(next, 4), false, -1, 0) &&
                        same(analyze(next, -1), false, -1, 0) &&
                        same(analyze(next, 5), false, -1, 0);
        report("branch-and-bounds", ok);
    }

    {
        const std::vector<int> empty;
        report("empty-graph", same(analyze(empty, 0), false, -1, 0));
    }

    {
        std::vector<int> next(10000);
        for (int i = 0; i + 1 < 5000; ++i) {
            next[static_cast<std::size_t>(i)] = i + 1;
        }
        for (int i = 5000; i + 1 < 10000; ++i) {
            next[static_cast<std::size_t>(i)] = i + 1;
        }
        next[4999] = 5000;
        next[9999] = 5000;
        report("large-tail", same(analyze(next, 0), true, 5000, 5000) &&
                                 same(analyze(next, 5000), true, 5000, 5000));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
