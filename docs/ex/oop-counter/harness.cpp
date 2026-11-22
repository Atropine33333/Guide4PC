// Guide4PC harness appended to the student's code in the same translation unit.
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
        Counter counter;
        report("default", counter.value() == 0);
    }

    {
        Counter positive(5);
        Counter negative(-3);
        Counter zero(0);
        report("start", positive.value() == 5 && negative.value() == 0 &&
                        zero.value() == 0);
    }

    {
        Counter counter(2);
        counter.increment();
        counter.increment();
        counter.decrement();
        const bool first = counter.value() == 3;
        counter.decrement();
        counter.decrement();
        counter.decrement();
        report("inc-dec", first && counter.value() == 0);
    }

    {
        Counter counter(1);
        Counter& reference = ++counter;
        const bool ok = counter.value() == 2 && &reference == &counter;
        ++counter;
        report("prefix", ok && counter.value() == 3);
    }

    {
        Counter counter(2);
        Counter old = counter++;
        const bool ok = old.value() == 2 && counter.value() == 3;
        Counter second = counter++;
        report("postfix", ok && second.value() == 3 && counter.value() == 4);
    }

    {
        Counter counter(5);
        Counter& reference = (counter += 3);
        const bool first = counter.value() == 8 && &reference == &counter;
        counter += -20;
        const bool second = counter.value() == 0;
        counter += 4;
        report("plus-equals", first && second && counter.value() == 4);
    }

    {
        Counter counter(9);
        counter.reset();
        report("reset", counter.value() == 0);
    }

    {
        Counter original(7);
        Counter copy = original;
        copy.increment();
        report("copy", original.value() == 7 && copy.value() == 8);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
