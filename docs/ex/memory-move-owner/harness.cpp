// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <utility>

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
        UniqueIntArray array(3);
        bool ok = array.size() == 3 && array.data() != nullptr;
        for (int i = 0; i < 3; ++i) ok = ok && array.get(i) == 0;
        array.set(0, 7);
        array.set(2, 9);
        ok = ok && array.get(0) == 7 && array.get(2) == 9;
        report("basic", ok);
    }

    {
        UniqueIntArray source(2);
        source.set(0, 4);
        UniqueIntArray target(std::move(source));
        report("move-ctor", target.size() == 2 && target.get(0) == 4 &&
                                target.data() != nullptr &&
                                source.size() == 0 && source.data() == nullptr);
    }

    {
        UniqueIntArray first(1);
        first.set(0, 1);
        UniqueIntArray second(4);
        second.set(3, 8);
        first = std::move(second);
        report("move-assign", first.size() == 4 && first.get(3) == 8 &&
                                  second.size() == 0 && second.data() == nullptr);
    }

    {
        UniqueIntArray empty(0);
        UniqueIntArray target(std::move(empty));
        report("zero", empty.size() == 0 && empty.data() == nullptr &&
                           target.size() == 0 && target.data() == nullptr);
    }

    {
        UniqueIntArray a(2);
        a.set(1, 6);
        UniqueIntArray b(std::move(a));
        UniqueIntArray c(std::move(b));
        report("chain", a.size() == 0 && b.size() == 0 && c.size() == 2 &&
                            c.get(1) == 6);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
