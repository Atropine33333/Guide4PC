// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <random>
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

}  // namespace

int main() {
    {
        IntList list;
        list.pushFront(1);
        list.pushFront(2);
        list.pushFront(3);
        int out = 0;
        const bool ok = list.size() == 3 && list.get(0, out) && out == 3 &&
                        list.get(1, out) && out == 2 && list.get(2, out) && out == 1;
        report("push-front-order", ok);
    }

    {
        IntList list;
        list.pushFront(5);
        list.pushFront(6);
        int out = 0;
        bool ok = list.popFront() && list.size() == 1 && list.get(0, out) && out == 5;
        ok = ok && list.popFront() && !list.popFront() && list.size() == 0;
        report("pop-front", ok);
    }

    {
        IntList list;
        for (int value : {1, 2, 3, 4}) list.pushFront(value);
        list.reverse();
        int out = 0;
        bool ok = list.size() == 4;
        for (int i = 0; i < 4; ++i) {
            ok = ok && list.get(i, out) && out == i + 1;
        }
        report("reverse-basic", ok);
    }

    {
        IntList list;
        list.reverse();
        int out = 0;
        bool ok = list.size() == 0 && !list.get(0, out);
        list.pushFront(7);
        list.reverse();
        ok = ok && list.size() == 1 && list.get(0, out) && out == 7;
        list.reverse();
        ok = ok && list.get(0, out) && out == 7;
        report("reverse-edge", ok);
    }

    {
        IntList list;
        list.pushFront(10);
        list.pushFront(20);
        int out = 0;
        bool ok = !list.get(-1, out) && !list.get(2, out) && !list.get(100, out);
        ok = ok && list.get(0, out) && out == 20 && list.size() == 2;
        report("get-bounds", ok);
    }

    {
        IntList list;
        std::vector<int> reference;
        std::mt19937 rng(918273645u);
        bool ok = true;
        for (int step = 0; step < 400 && ok; ++step) {
            const int action = static_cast<int>(rng() % 5u);
            if (action <= 1) {
                const int value = static_cast<int>(rng() % 41u) - 20;
                list.pushFront(value);
                reference.insert(reference.begin(), value);
            } else if (action == 2) {
                const bool expected = !reference.empty();
                if (expected) reference.erase(reference.begin());
                ok = ok && list.popFront() == expected;
            } else if (action == 3) {
                list.reverse();
                std::vector<int> reversed(reference.rbegin(), reference.rend());
                reference.swap(reversed);
            } else {
                int index = static_cast<int>(rng() % 12u) - 2;
                int out = 0;
                const bool expected = index >= 0 && index < static_cast<int>(reference.size());
                ok = ok && list.get(index, out) == expected;
                if (expected) ok = ok && out == reference[static_cast<std::size_t>(index)];
            }
            ok = ok && list.size() == static_cast<int>(reference.size());
        }
        report("random-trace", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
