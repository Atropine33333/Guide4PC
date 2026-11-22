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
        IntBuffer buffer(3);
        int value = -1;
        bool ok = buffer.size() == 3 && buffer.get(0, value) && value == 0;
        ok = ok && buffer.set(1, 42) && buffer.get(1, value) && value == 42;
        report("set-get", ok);
    }

    {
        IntBuffer buffer(2);
        int value = 0;
        bool ok = !buffer.set(-1, 5) && !buffer.set(2, 5);
        ok = ok && !buffer.get(-1, value) && !buffer.get(2, value);
        ok = ok && buffer.set(1, 8) && buffer.get(1, value) && value == 8;
        report("bounds", ok);
    }

    {
        IntBuffer original(3);
        original.fill(7);
        IntBuffer copy(original);
        copy.set(0, 99);
        int value = 0;
        const bool ok = copy.size() == 3 && copy.get(0, value) && value == 99 &&
                        original.get(0, value) && value == 7 &&
                        original.get(2, value) && value == 7;
        report("copy-independent", ok);
    }

    {
        IntBuffer source(2);
        source.fill(4);
        IntBuffer target(5);
        target.fill(1);
        target = source;
        target.set(0, 6);
        int value = 0;
        const bool ok = target.size() == 2 && target.get(0, value) && value == 6 &&
                        source.get(0, value) && value == 4 &&
                        !target.get(2, value);
        report("assignment-independent", ok);
    }

    {
        IntBuffer buffer(2);
        buffer.set(0, 11);
        buffer.set(1, 22);
        IntBuffer *alias = &buffer;
        buffer = *alias;
        int value = 0;
        const bool ok = buffer.size() == 2 && buffer.get(0, value) && value == 11 &&
                        buffer.get(1, value) && value == 22;
        report("self-assignment", ok);
    }

    {
        IntBuffer empty(0);
        IntBuffer copied(empty);
        IntBuffer assigned(3);
        assigned = empty;
        int value = 0;
        empty.fill(9);
        const bool ok = empty.size() == 0 && copied.size() == 0 && assigned.size() == 0 &&
                        !empty.get(0, value) && !copied.get(0, value) && !assigned.get(0, value);
        report("empty-buffer", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
