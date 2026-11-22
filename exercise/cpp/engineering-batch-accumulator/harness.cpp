// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
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

bool same(const std::vector<int>& actual, const std::vector<int>& expected) {
    return actual == expected;
}

}  // namespace

int main() {
    {
        BatchAccumulator accumulator(3);
        const bool first = accumulator.add(1);
        const bool second = accumulator.add(2);
        const bool early = accumulator.take_batch().empty();
        const bool third = accumulator.add(3);
        const bool overflow = accumulator.add(4);
        const std::vector<int> batch = accumulator.take_batch();
        const bool fourth = accumulator.add(4);
        const bool fifth = accumulator.add(5);
        const std::vector<int> tail = accumulator.flush();
        report("basic", first && second && early && third && !overflow &&
                            same(batch, {1, 2, 3}) && fourth && fifth &&
                            same(tail, {4, 5}) && accumulator.pending() == 0 &&
                            accumulator.emitted() == 5);
    }

    {
        BatchAccumulator accumulator(1);
        const bool one = accumulator.add(-4);
        const std::vector<int> first = accumulator.take_batch();
        const bool two = accumulator.add(9);
        const std::vector<int> second = accumulator.take_batch();
        report("single", one && two && same(first, {-4}) &&
                             same(second, {9}) && accumulator.emitted() == 2);
    }

    {
        BatchAccumulator accumulator(4);
        const bool early = accumulator.take_batch().empty();
        const bool empty = accumulator.flush().empty();
        report("empty", early && empty && accumulator.pending() == 0 &&
                            accumulator.emitted() == 0);
    }

    {
        BatchAccumulator accumulator(3);
        bool added = true;
        for (int i = 1; i <= 3; ++i) added = added && accumulator.add(i);
        const std::vector<int> first = accumulator.take_batch();
        for (int i = 4; i <= 6; ++i) added = added && accumulator.add(i);
        const std::vector<int> second = accumulator.take_batch();
        added = added && accumulator.add(7);
        const std::vector<int> last = accumulator.flush();
        report("multiple", added && same(first, {1, 2, 3}) &&
                               same(second, {4, 5, 6}) &&
                               same(last, {7}) &&
                               accumulator.emitted() == 7);
    }

    {
        BatchAccumulator accumulator(0);
        const bool added = accumulator.add(42);
        const std::vector<int> batch = accumulator.take_batch();
        report("clamp", added && same(batch, {42}) &&
                            accumulator.emitted() == 1);
    }

    {
        BatchAccumulator accumulator(2);
        accumulator.add(-1);
        accumulator.add(-2);
        const std::vector<int> batch = accumulator.take_batch();
        accumulator.add(-3);
        const std::vector<int> tail = accumulator.flush();
        report("negative", same(batch, {-1, -2}) && same(tail, {-3}) &&
                               accumulator.emitted() == 3);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
