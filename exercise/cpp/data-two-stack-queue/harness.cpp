// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>

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
        const bool ok = q.empty() && q.size() == 0 && q.front() == -1 &&
                        q.min() == -1;
        report("empty-behaviour", ok);
    }

    {
        TwoStackQueue q;
        q.push(3);
        q.push(1);
        q.push(2);
        bool ok = q.front() == 3 && q.min() == 1 && q.size() == 3;
        q.pop();
        ok = ok && q.front() == 1 && q.min() == 1 && q.size() == 2;
        q.pop();
        ok = ok && q.front() == 2 && q.min() == 2 && q.size() == 1;
        q.pop();
        ok = ok && q.empty() && q.front() == -1 && q.min() == -1;
        report("fifo-order", ok);
    }

    {
        TwoStackQueue q;
        bool ok = true;
        q.push(5);
        ok = ok && q.min() == 5;
        q.push(3);
        ok = ok && q.min() == 3;
        q.push(4);
        ok = ok && q.min() == 3;
        q.push(3);
        ok = ok && q.min() == 3 && q.front() == 5;
        report("min-tracks", ok);
    }

    {
        TwoStackQueue q;
        q.push(2);
        q.push(5);
        q.push(1);
        q.push(4);
        bool ok = q.min() == 1;
        q.pop();
        ok = ok && q.min() == 1;
        q.pop();
        ok = ok && q.min() == 1;
        q.pop();
        ok = ok && q.min() == 4 && q.front() == 4;
        q.pop();
        ok = ok && q.empty() && q.min() == -1;
        report("min-after-pop", ok);
    }

    {
        TwoStackQueue q;
        q.push(10);
        q.push(20);
        q.push(30);
        bool ok = q.min() == 10;
        q.pop();
        ok = ok && q.front() == 20 && q.min() == 20;
        q.push(5);
        ok = ok && q.min() == 5;
        q.pop();
        ok = ok && q.front() == 30 && q.min() == 5;
        q.pop();
        ok = ok && q.front() == 5 && q.min() == 5;
        q.pop();
        ok = ok && q.empty() && q.min() == -1;
        report("min-after-refill", ok);
    }

    {
        TwoStackQueue q;
        bool ok = true;
        int lowest = 1000000000;
        for (int i = 0; i < 1000; ++i) {
            const int value = (i * 37) % 1000;
            if (value < lowest) lowest = value;
            q.push(value);
            ok = ok && q.min() == lowest;
        }
        ok = ok && q.size() == 1000;
        for (int i = 0; i < 1000; ++i) {
            const int value = (i * 37) % 1000;
            ok = ok && q.front() == value;
            q.pop();
        }
        ok = ok && q.empty() && q.min() == -1;
        report("large-fifo", ok);
    }

    {
        TwoStackQueue original;
        original.push(4);
        original.push(2);
        TwoStackQueue copy = original;
        copy.push(1);
        const bool ok = original.min() == 2 && original.size() == 2 &&
                        copy.min() == 1 && copy.size() == 3 &&
                        original.front() == 4 && copy.front() == 4;
        report("copy-independent", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
