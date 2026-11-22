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
        RetryPolicy policy(100, 1000, 0);
        report("no-attempts", !policy.has_next() && policy.next_delay() == -1 &&
                                  policy.attempts() == 0);
    }

    {
        RetryPolicy policy(100, 1000, 5);
        const bool ok = policy.has_next() && policy.next_delay() == 100 &&
                        policy.next_delay() == 200 &&
                        policy.next_delay() == 400 &&
                        policy.next_delay() == 800 &&
                        policy.next_delay() == 1000 && !policy.has_next() &&
                        policy.attempts() == 5 && policy.next_delay() == -1 &&
                        policy.attempts() == 5;
        report("sequence", ok);
    }

    {
        RetryPolicy policy(0, 5, 3);
        report("zero-base", policy.next_delay() == 0 &&
                                 policy.next_delay() == 0 &&
                                 policy.next_delay() == 0 &&
                                 !policy.has_next());
    }

    {
        RetryPolicy policy(7, 7, 4);
        report("flat", policy.next_delay() == 7 && policy.next_delay() == 7 &&
                           policy.next_delay() == 7 &&
                           policy.next_delay() == 7 && policy.next_delay() == -1);
    }

    {
        RetryPolicy policy(100, 1000, 5);
        policy.next_delay();
        policy.next_delay();
        policy.reset();
        report("reset", policy.attempts() == 0 && policy.has_next() &&
                            policy.next_delay() == 100 && policy.attempts() == 1);
    }

    {
        RetryPolicy policy(1000000000, 2000000000, 4);
        const bool ok = policy.next_delay() == 1000000000 &&
                        policy.next_delay() == 2000000000 &&
                        policy.next_delay() == 2000000000 &&
                        policy.next_delay() == 2000000000 &&
                        policy.next_delay() == -1;
        report("cap", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
