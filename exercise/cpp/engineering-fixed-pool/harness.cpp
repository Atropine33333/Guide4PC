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
        IntPool pool(0);
        report("zero-capacity", pool.capacity() == 0 && pool.live() == 0 &&
                                pool.acquire(5) == nullptr &&
                                pool.release(nullptr) == false);
    }

    {
        IntPool pool(3);
        int *first = pool.acquire(10);
        int *second = pool.acquire(20);
        const bool ok = first != nullptr && second != nullptr && first != second &&
                        *first == 10 && *second == 20 && pool.live() == 2 &&
                        pool.capacity() == 3;
        report("acquire", ok);
    }

    {
        IntPool pool(3);
        int *first = pool.acquire(1);
        int *second = pool.acquire(2);
        int *third = pool.acquire(3);
        int *fourth = pool.acquire(4);
        const bool ok = first && second && third && fourth == nullptr &&
                        pool.live() == 3 && *first == 1 && *second == 2 && *third == 3;
        report("full", ok);
    }

    {
        IntPool pool(2);
        int *first = pool.acquire(10);
        int *second = pool.acquire(20);
        const bool released = pool.release(first);
        const bool reused = pool.acquire(30) == first && *first == 30 &&
                            pool.live() == 2;
        report("reuse", released && reused && *second == 20);
    }

    {
        IntPool pool(2);
        int outside = 7;
        int *slot = pool.acquire(1);
        const bool ok = pool.release(&outside) == false &&
                        pool.release(nullptr) == false &&
                        pool.live() == 1 && *slot == 1;
        report("foreign", ok);
    }

    {
        IntPool pool(2);
        int *slot = pool.acquire(1);
        const bool first = pool.release(slot) == true && pool.live() == 0;
        const bool second = pool.release(slot) == false && pool.live() == 0;
        report("double-release", first && second);
    }

    {
        IntPool pool(3);
        int *first = pool.acquire(1);
        int *second = pool.acquire(2);
        int *third = pool.acquire(3);
        const bool ok = first && second && third && pool.live() == 3 &&
                        pool.release(second) && pool.live() == 2 &&
                        pool.acquire(9) == second && pool.live() == 3;
        report("release-middle", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
