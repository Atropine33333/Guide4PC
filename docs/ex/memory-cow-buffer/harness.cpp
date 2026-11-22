// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <string>

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
    const int base = CowBuffer::live_blocks();

    {
        CowBuffer empty;
        const bool empty_owns = empty.str().empty() && empty.shares() == 1 &&
                                CowBuffer::live_blocks() == base + 1;
        CowBuffer first("abc");
        const bool initial = empty_owns && first.str() == "abc" &&
                             first.shares() == 1 &&
                             CowBuffer::live_blocks() == base + 2;

        CowBuffer second = first;
        const bool copy = first.shares() == 2 && second.shares() == 2 &&
                          CowBuffer::live_blocks() == base + 2 &&
                          second.str() == "abc";

        second.append("d");
        const bool detach = first.str() == "abc" && second.str() == "abcd" &&
                            first.shares() == 1 && second.shares() == 1 &&
                            CowBuffer::live_blocks() == base + 3;
        report("initial", initial);
        report("copy", copy);
        report("detach", detach);
    }
    const bool first_released = CowBuffer::live_blocks() == base;

    {
        CowBuffer a("x");
        CowBuffer b("y");
        const int two_blocks = CowBuffer::live_blocks();
        b = a;
        const bool assigned = b.str() == "x" && a.shares() == 2 &&
                              CowBuffer::live_blocks() == two_blocks - 1;
        CowBuffer& alias = b;
        b = alias;
        const bool self = b.str() == "x" && a.shares() == 2 &&
                          CowBuffer::live_blocks() == two_blocks - 1;
        b.append("z");
        const bool split = a.str() == "x" && b.str() == "xz" &&
                           CowBuffer::live_blocks() == two_blocks;

        bool shared = false;
        {
            CowBuffer c = a;
            CowBuffer d = b;
            shared = a.shares() == 2 && b.shares() == 2;
        }
        const bool after_scope = a.shares() == 1 && b.shares() == 1;
        report("assign", assigned && self && split && shared && after_scope);
    }

    {
        CowBuffer a;
        CowBuffer b;
        b = a;
        b.append("tail");
        report("empty", a.str().empty() && b.str() == "tail" &&
                            a.shares() == 1 && b.shares() == 1);
    }

    report("destroy", first_released && CowBuffer::live_blocks() == base);

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
