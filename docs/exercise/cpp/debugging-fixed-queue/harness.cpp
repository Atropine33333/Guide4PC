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
        FixedQueue queue(3);
        int value = 99;
        const bool ok = queue.capacity() == 3 && queue.size() == 0 &&
                        queue.pop(value) == false && value == 99;
        report("empty", ok);
        queue.clear();
        report("empty-clear", queue.size() == 0 && queue.pop(value) == false);
    }

    {
        FixedQueue queue(3);
        int value = 0;
        const bool pushed = queue.push(5) == true && queue.size() == 1;
        const bool popped = queue.pop(value) == true && value == 5 && queue.size() == 0;
        report("singleton", pushed && popped && queue.pop(value) == false);
    }

    {
        FixedQueue queue(3);
        queue.push(1);
        queue.push(2);
        queue.push(3);
        int first = 0;
        int second = 0;
        const bool order = queue.pop(first) && first == 1 &&
                           queue.pop(second) && second == 2 &&
                           queue.size() == 1;
        queue.push(4);
        int third = 0;
        int fourth = 0;
        const bool rest = queue.pop(third) && third == 3 &&
                          queue.pop(fourth) && fourth == 4 &&
                          queue.size() == 0;
        report("fifo", order && rest);
    }

    {
        FixedQueue queue(3);
        queue.push(1);
        queue.push(2);
        int value = 0;
        queue.pop(value);
        queue.push(3);
        queue.push(4);
        const bool ok = value == 1 && queue.pop(value) && value == 2 &&
                        queue.pop(value) && value == 3 &&
                        queue.pop(value) && value == 4 &&
                        queue.pop(value) == false;
        report("wrap", ok);
    }

    {
        FixedQueue queue(3);
        queue.push(1);
        queue.push(2);
        queue.push(3);
        const bool rejected = queue.push(4) == false;
        int value = 0;
        const bool kept = queue.size() == 3 && queue.pop(value) && value == 1 &&
                          queue.pop(value) && value == 2 &&
                          queue.pop(value) && value == 3 &&
                          queue.pop(value) == false;
        report("full", rejected && kept);
    }

    {
        FixedQueue queue(0);
        int value = 7;
        const bool ok = queue.capacity() == 0 && queue.size() == 0 &&
                        queue.push(1) == false && queue.pop(value) == false &&
                        value == 7;
        queue.clear();
        report("zero-capacity", ok && queue.size() == 0 && queue.push(2) == false);
    }

    {
        FixedQueue queue(4);
        queue.push(1);
        queue.push(2);
        queue.clear();
        int value = 0;
        const bool cleared = queue.size() == 0 && queue.pop(value) == false;
        const bool reused = queue.push(9) == true && queue.pop(value) &&
                            value == 9 && queue.size() == 0;
        report("clear", cleared && reused);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
