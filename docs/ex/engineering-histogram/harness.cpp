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
        const Histogram empty(4, 10);
        report("empty", empty.buckets() == 4 && empty.width() == 10 &&
                            empty.total() == 0 && empty.overflow() == 0 &&
                            empty.count(0) == 0 && empty.count(3) == 0);
    }

    {
        Histogram histogram(4, 10);
        histogram.add(0);
        histogram.add(9);
        histogram.add(10);
        histogram.add(35);
        histogram.add(39);
        const bool ok = histogram.count(0) == 2 && histogram.count(1) == 1 &&
                        histogram.count(2) == 0 && histogram.count(3) == 2 &&
                        histogram.total() == 5 && histogram.overflow() == 0;
        report("bucket-counts", ok);
    }

    {
        Histogram histogram(4, 10);
        histogram.add(39);
        histogram.add(40);
        histogram.add(45);
        histogram.add(-1);
        histogram.add(-100);
        const bool ok = histogram.count(3) == 1 && histogram.overflow() == 2 &&
                        histogram.total() == 3;
        report("overflow-and-negative", ok);
    }

    {
        Histogram histogram(3, 5);
        histogram.add(4);
        histogram.add(5);
        histogram.add(14);
        histogram.add(15);
        histogram.add(-5);
        const bool ok = histogram.count(0) == 1 && histogram.count(1) == 1 &&
                        histogram.count(2) == 1 && histogram.overflow() == 1 &&
                        histogram.total() == 4;
        report("width-boundaries", ok);
    }

    {
        Histogram histogram(10, 10);
        for (int value = 0; value < 100; ++value) {
            histogram.add(value);
        }
        bool ok = histogram.total() == 100 && histogram.overflow() == 0;
        for (int bucket = 0; bucket < 10; ++bucket) {
            ok = ok && histogram.count(bucket) == 10;
        }
        report("unit-width", ok);
    }

    {
        Histogram histogram(2, 3);
        histogram.add(-1);
        histogram.add(-2);
        const bool ok = histogram.total() == 0 && histogram.overflow() == 0 &&
                        histogram.count(-1) == 0 && histogram.count(2) == 0 &&
                        histogram.count(100) == 0;
        report("ignored-and-out-of-range", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
