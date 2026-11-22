// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
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

bool same(const std::vector<int> &actual, const std::vector<int> &expected) {
    return actual == expected;
}

}  // namespace

int main() {
    {
        Pager pager(95, 10, 3);
        const bool ok = pager.page_count() == 10 && pager.current() == 3 &&
                        pager.first_index() == 20 && pager.last_index() == 29 &&
                        pager.has_prev() && pager.has_next();
        report("basic", ok);
    }

    {
        Pager high(95, 10, 99);
        Pager low(95, 10, -4);
        const bool ok = high.current() == 10 && !high.has_next() && high.last_index() == 94 &&
                        low.current() == 1 && !low.has_prev() && low.first_index() == 0;
        report("clamp", ok);
    }

    {
        Pager pager(0, 10, 5);
        const bool ok = pager.page_count() == 0 && pager.current() == 1 &&
                        pager.first_index() == -1 && pager.last_index() == -1 &&
                        !pager.has_prev() && !pager.has_next() && pager.window(3).empty();
        report("empty", ok);
    }

    {
        Pager pager(50, 10, 5);
        const bool ok = pager.page_count() == 5 && pager.first_index() == 40 &&
                        pager.last_index() == 49 && !pager.has_next();
        report("exact-multiple", ok);
    }

    {
        Pager pager(7, 10, 1);
        const bool ok = pager.page_count() == 1 && pager.first_index() == 0 &&
                        pager.last_index() == 6 && !pager.has_prev() && !pager.has_next();
        report("single-page", ok);
    }

    {
        Pager middle(95, 10, 5);
        Pager start(95, 10, 1);
        Pager end(95, 10, 10);
        Pager zero(95, 10, 5);
        const bool ok = same(middle.window(2), {3, 4, 5, 6, 7}) &&
                        same(start.window(2), {1, 2, 3}) &&
                        same(end.window(2), {8, 9, 10}) && same(zero.window(0), {5}) &&
                        same(zero.window(-3), {5});
        report("window", ok);
    }

    {
        Pager pager(3, 1, 2);
        const bool ok = pager.page_count() == 3 && pager.first_index() == 1 &&
                        pager.last_index() == 1 && pager.has_prev() && pager.has_next() &&
                        same(pager.window(1), {1, 2, 3});
        report("page-size-one", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
