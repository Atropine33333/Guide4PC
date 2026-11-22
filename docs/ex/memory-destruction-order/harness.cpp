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

bool same(const std::vector<int>& actual, const std::vector<int>& expected) {
    return actual == expected;
}

}  // namespace

int main() {
    {
        ScopeTracer::clear_log();
        {
            ScopeTracer a(1);
        }
        report("basic", same(ScopeTracer::destroyed_log(), {1}));
    }

    {
        ScopeTracer::clear_log();
        {
            ScopeTracer a(1);
            ScopeTracer b(2);
            ScopeTracer c(3);
        }
        report("reverse", same(ScopeTracer::destroyed_log(), {3, 2, 1}));
    }

    {
        ScopeTracer::clear_log();
        {
            ScopeTracer outer(1);
            {
                ScopeTracer inner_a(2);
                ScopeTracer inner_b(3);
            }
        }
        report("nested", same(ScopeTracer::destroyed_log(), {3, 2, 1}));
    }

    {
        ScopeTracer::clear_log();
        {
            ScopeTracer* first = new ScopeTracer(4);
            ScopeTracer* second = new ScopeTracer(5);
            delete first;
            delete second;
        }
        report("heap", same(ScopeTracer::destroyed_log(), {4, 5}));
    }

    {
        ScopeTracer::clear_log();
        {
            ScopeTracer a(1);
        }
        {
            ScopeTracer b(2);
        }
        report("separate", same(ScopeTracer::destroyed_log(), {1, 2}));
    }

    {
        ScopeTracer::clear_log();
        {
            ScopeTracer a(7);
        }
        ScopeTracer::clear_log();
        {
            ScopeTracer b(9);
        }
        report("clear", same(ScopeTracer::destroyed_log(), {9}));
    }

    {
        ScopeTracer::clear_log();
        ScopeTracer a(42);
        report("id", a.id() == 42 && ScopeTracer::destroyed_log().empty());
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
