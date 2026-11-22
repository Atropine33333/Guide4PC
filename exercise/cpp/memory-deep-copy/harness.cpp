// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstddef>
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
        IntBuffer buffer(0);
        report("size-zero", buffer.size() == 0);
    }

    {
        IntBuffer buffer(4);
        bool ok = buffer.size() == 4;
        for (std::size_t i = 0; i < 4; ++i) {
            if (buffer.at(i) != 0) ok = false;
        }
        report("zero-init", ok);
    }

    {
        IntBuffer buffer(3);
        buffer.at(1) = 7;
        const bool ok = buffer.at(1) == 7 && buffer.at(0) == 0 && buffer.size() == 3;
        report("write-read", ok);
    }

    {
        IntBuffer original(3);
        for (std::size_t i = 0; i < 3; ++i) original.at(i) = static_cast<int>(i + 1);
        IntBuffer copy(original);
        copy.at(0) = 99;
        original.at(1) = 42;
        const bool ok = copy.size() == original.size() && copy.at(0) == 99 &&
                        copy.at(1) == 2 && original.at(0) == 1 && original.at(1) == 42;
        report("copy-independent", ok);
    }

    {
        IntBuffer small(2);
        small.at(0) = 1;
        small.at(1) = 2;
        IntBuffer large(4);
        for (std::size_t i = 0; i < 4; ++i) large.at(i) = 5;
        small = large;
        large.at(0) = 8;
        const bool ok = small.size() == 4 && small.at(0) == 5 && small.at(3) == 5 &&
                        large.at(0) == 8;
        report("assign", ok);
    }

    {
        IntBuffer buffer(2);
        buffer.at(0) = 3;
        buffer.at(1) = 4;
        IntBuffer& reference = buffer;
        buffer = reference;
        const bool ok = buffer.size() == 2 && buffer.at(0) == 3 && buffer.at(1) == 4;
        report("self-assign", ok);
    }

    {
        IntBuffer buffer(2);
        buffer.at(0) = 11;
        const IntBuffer& constant = buffer;
        const bool ok = constant.size() == 2 && constant.at(0) == 11 && constant.at(1) == 0;
        report("const-access", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
