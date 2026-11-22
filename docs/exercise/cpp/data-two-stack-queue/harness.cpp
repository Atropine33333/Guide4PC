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

}  // namespace

int main() {
    {
        TwoStackQueue q;
        q.pop();
        const bool ok = q.empty() && q.size() == 0 && q.front() == -1;
        report("empty-behaviour", ok);
    }

    {
        TwoStackQueue q;
        q.push(1);
        q.push(2);
        q.push(3);
        bool ok = q.front() == 1 && q.size() == 3;
        q.pop();
        ok = ok && q.front() == 2 && q.size() == 2;
        q.pop();
        ok = ok && q.front() == 3 && q.size() == 1;
        q.pop();
        ok = ok && q.empty() && q.front() == -1;
        report("fifo-order", ok);
    }

    {
        TwoStackQueue q;
        bool ok = true;
        q.push(1);
        ok = ok && q.front() == 1;
        q.pop();
        q.push(2);
        q.push(3);
        ok = ok && q.front() == 2;
        q.pop();
        q.push(4);
        ok = ok && q.front() == 3 && q.size() == 2;
        q.pop();
        ok = ok && q.front() == 4;
        q.pop();
        ok = ok && q.empty();
        report("interleaved", ok);
    }

    {
        TwoStackQueue q;
        for (int i = 0; i < 1000; ++i) {
            q.push(i);
        }
        bool ok = q.size() == 1000;
        for (int i = 0; i < 1000; ++i) {
            ok = ok && q.front() == i;
            q.pop();
        }
        ok = ok && q.empty();
        report("large-fifo", ok);
    }

    {
        TwoStackQueue q;
        bool ok = true;
        for (int round = 0; round < 100; ++round) {
            q.push(round);
            ok = ok && q.front() == round && q.size() == 1;
            q.pop();
            ok = ok && q.empty();
        }
        report("alternating", ok);
    }

    {
        TwoStackQueue original;
        original.push(5);
        original.push(6);
        TwoStackQueue copy = original;
        copy.push(7);
        const bool ok = original.size() == 2 && original.front() == 5 &&
                        copy.size() == 3 && copy.front() == 5;
        report("copy-independent", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
