// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <set>
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

struct h_rng {
    std::uint32_t state;
    std::uint32_t next() {
        state = state * 1664525u + 1013904223u;
        return state >> 8;
    }
    int below(int limit) { return static_cast<int>(next() % static_cast<std::uint32_t>(limit)); }
};

bool h_drain_equals(MinHeap &heap, const std::vector<int> &expected) {
    for (int value : expected) {
        if (heap.empty()) return false;
        if (heap.top() != value) return false;
        if (heap.pop() != value) return false;
    }
    return heap.empty();
}

std::vector<int> h_sorted(std::vector<int> values) {
    for (std::size_t i = 0; i < values.size(); ++i) {
        for (std::size_t j = i + 1; j < values.size(); ++j) {
            if (values[j] < values[i]) {
                const int tmp = values[i];
                values[i] = values[j];
                values[j] = tmp;
            }
        }
    }
    return values;
}

}  // namespace

int main() {
    {
        MinHeap heap;
        report("empty", heap.empty() && heap.size() == 0);
    }

    {
        MinHeap heap;
        heap.push(42);
        const bool ok = !heap.empty() && heap.size() == 1 && heap.top() == 42 && heap.pop() == 42 &&
                        heap.empty() && heap.size() == 0;
        report("single", ok);
    }

    {
        MinHeap heap;
        for (int value : {5, 1, 4, 2, 3}) heap.push(value);
        const bool ok = heap.size() == 5 && h_drain_equals(heap, {1, 2, 3, 4, 5});
        report("ordering", ok);
    }

    {
        MinHeap heap;
        for (int value : {2, 2, 1, 2, 1}) heap.push(value);
        const bool ok = h_drain_equals(heap, {1, 1, 2, 2, 2});
        report("duplicates", ok);
    }

    {
        MinHeap heap;
        for (int value : {-5, 0, -10, 3, -10, 7}) heap.push(value);
        const bool ok = h_drain_equals(heap, {-10, -10, -5, 0, 3, 7});
        report("negative", ok);
    }

    {
        h_rng rng{9001u};
        MinHeap heap;
        std::vector<int> values;
        for (int i = 0; i < 300; ++i) {
            const int value = rng.below(1000) - 500;
            values.push_back(value);
            heap.push(value);
        }
        const bool ok = heap.size() == values.size() && h_drain_equals(heap, h_sorted(values));
        report("random", ok);
    }

    {
        h_rng rng{424242u};
        MinHeap heap;
        std::multiset<int> oracle;
        bool ok = true;
        for (int step = 0; step < 500 && ok; ++step) {
            const bool do_push = oracle.empty() || rng.below(3) != 0;
            if (do_push) {
                const int value = rng.below(200) - 100;
                heap.push(value);
                oracle.insert(value);
            } else {
                const int expected = *oracle.begin();
                oracle.erase(oracle.begin());
                if (heap.top() != expected || heap.pop() != expected) ok = false;
            }
            if (ok && heap.size() != oracle.size()) ok = false;
        }
        while (ok && !oracle.empty()) {
            const int expected = *oracle.begin();
            oracle.erase(oracle.begin());
            if (heap.top() != expected || heap.pop() != expected) ok = false;
        }
        report("interleaved", ok && heap.empty());
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
