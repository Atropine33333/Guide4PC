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

void reset() {
    Tracked::copies = 0;
    Tracked::moves = 0;
}

struct Convertible {
    int number;
    operator int() const { return number; }
};

}  // namespace

int main() {
    {
        reset();
        Tracked source(5);
        Tracked result = forward_to_tracked(source);
        report("lvalue-copy", result.value == 5 && Tracked::copies == 1 &&
                                   Tracked::moves == 0 && source.value == 5);
    }

    {
        reset();
        Tracked result = forward_to_tracked(Tracked(7));
        report("rvalue-move", result.value == 7 && Tracked::copies == 0 &&
                                  Tracked::moves == 1);
    }

    {
        reset();
        Tracked result = forward_to_tracked(42);
        report("int-direct", result.value == 42 && Tracked::copies == 0 &&
                                 Tracked::moves == 0);
    }

    {
        reset();
        const Tracked source(9);
        Tracked result = forward_to_tracked(source);
        report("const-lvalue-copy", result.value == 9 && Tracked::copies == 1 &&
                                       Tracked::moves == 0);
    }

    {
        reset();
        const Convertible input{31};
        Tracked result = forward_to_tracked(input);
        report("convertible-direct", result.value == 31 &&
                                         Tracked::copies == 0 &&
                                         Tracked::moves == 0);
    }

    {
        reset();
        Tracked first(1);
        Tracked second(2);
        Tracked a = forward_to_tracked(first);
        Tracked b = forward_to_tracked(std::move(second));
        report("counters-reset-between", a.value == 1 && b.value == 2 &&
                                             second.value == 0 &&
                                             Tracked::copies == 1 &&
                                             Tracked::moves == 1);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
