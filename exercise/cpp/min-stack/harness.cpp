// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <random>
#include <utility>
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
        MinStack stack;
        stack.push(5);
        stack.push(3);
        int top = 0;
        report("push-top", stack.size() == 2 && stack.top(top) && top == 3);
    }

    {
        MinStack stack;
        stack.push(1);
        stack.push(2);
        int top = 0;
        bool ok = stack.pop() && stack.top(top) && top == 1;
        ok = ok && stack.pop() && !stack.pop() && stack.size() == 0;
        report("pop-order", ok);
    }

    {
        MinStack stack;
        for (int value : {5, 2, 7, 2, 1}) stack.push(value);
        int minimum = 0;
        bool ok = stack.min(minimum) && minimum == 1;
        ok = ok && stack.pop() && stack.min(minimum) && minimum == 2;
        ok = ok && stack.pop() && stack.min(minimum) && minimum == 2;
        ok = ok && stack.pop() && stack.min(minimum) && minimum == 2;
        ok = ok && stack.pop() && stack.min(minimum) && minimum == 5;
        report("minimum-tracking", ok);
    }

    {
        MinStack stack;
        stack.push(3);
        stack.push(3);
        stack.push(4);
        int minimum = 0;
        bool ok = stack.pop() && stack.min(minimum) && minimum == 3;
        ok = ok && stack.pop() && stack.min(minimum) && minimum == 3;
        ok = ok && stack.pop() && !stack.min(minimum) && stack.size() == 0;
        report("duplicate-minimum", ok);
    }

    {
        MinStack stack;
        int value = 0;
        bool ok = !stack.pop() && !stack.top(value) && !stack.min(value) && stack.size() == 0;
        stack.push(9);
        ok = ok && stack.pop() && !stack.pop() && !stack.top(value);
        report("empty-operations", ok);
    }

    {
        MinStack stack;
        std::vector<int> values;
        std::mt19937 rng(20240517u);
        bool ok = true;
        for (int step = 0; step < 400 && ok; ++step) {
            const int action = static_cast<int>(rng() % 4u);
            if (action == 0) {
                const int value = static_cast<int>(rng() % 21u) - 10;
                stack.push(value);
                values.push_back(value);
            } else if (action == 1) {
                const bool expected = !values.empty();
                if (!values.empty()) values.pop_back();
                ok = ok && stack.pop() == expected;
            } else if (action == 2) {
                int top = 0;
                const bool expected = !values.empty();
                ok = ok && stack.top(top) == expected;
                if (expected) ok = ok && top == values.back();
            } else {
                int minimum = 0;
                const bool expected = !values.empty();
                ok = ok && stack.min(minimum) == expected;
                if (expected) {
                    int best = values.front();
                    for (int value : values) best = value < best ? value : best;
                    ok = ok && minimum == best;
                }
            }
            ok = ok && stack.size() == static_cast<int>(values.size());
        }
        report("random-trace", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
