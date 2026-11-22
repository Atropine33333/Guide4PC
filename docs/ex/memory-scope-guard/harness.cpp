// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
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

void h_count(void *context) { ++*static_cast<int *>(context); }

void h_push1(void *context) { static_cast<std::vector<int> *>(context)->push_back(1); }
void h_push2(void *context) { static_cast<std::vector<int> *>(context)->push_back(2); }
void h_push3(void *context) { static_cast<std::vector<int> *>(context)->push_back(3); }

}  // namespace

int main() {
    static_assert(!std::is_copy_constructible_v<ScopeGuard>);
    static_assert(!std::is_copy_assignable_v<ScopeGuard>);

    {
        int count = 0;
        {
            ScopeGuard guard(h_count, &count);
            if (count != 0) count = -1000;
        }
        report("destructor", count == 1);
    }

    {
        int count = 0;
        {
            ScopeGuard guard(h_count, &count);
            guard.dismiss();
        }
        report("dismiss", count == 0);
    }

    {
        std::vector<int> log;
        {
            ScopeGuard first(h_push1, &log);
            ScopeGuard second(h_push2, &log);
            ScopeGuard third(h_push3, &log);
            (void)first;
            (void)second;
            (void)third;
        }
        const bool ok = log.size() == 3 && log[0] == 3 && log[1] == 2 && log[2] == 1;
        report("reverse-order", ok);
    }

    {
        std::vector<int> log;
        {
            ScopeGuard outer(h_push1, &log);
            {
                (void)outer;
                ScopeGuard inner(h_push2, &log);
                log.push_back(0);
            }
        }
        const bool ok = log.size() == 3 && log[0] == 0 && log[1] == 2 && log[2] == 1;
        report("nested", ok);
    }

    {
        const bool deleted = !std::is_copy_constructible_v<ScopeGuard> &&
                             !std::is_copy_assignable_v<ScopeGuard>;
        report("copy-deleted", deleted);
    }

    {
        int count = 0;
        {
            ScopeGuard guard(h_count, &count);
            guard.dismiss();
            guard.dismiss();
        }
        report("dismiss-twice", count == 0);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
