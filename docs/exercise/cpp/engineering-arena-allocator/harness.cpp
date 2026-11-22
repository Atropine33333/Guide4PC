// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <cstdint>
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

bool aligned(const void* pointer, int alignment) {
    return pointer != nullptr &&
           reinterpret_cast<std::uintptr_t>(pointer) % static_cast<std::uintptr_t>(alignment) == 0;
}

}  // namespace

int main() {
    {
        Arena arena(256);
        report("empty", arena.capacity() == 256 && arena.used() == 0);
    }

    {
        Arena arena(256);
        void* first = arena.allocate(16, 8);
        void* second = arena.allocate(8, 4);
        report("basic", aligned(first, 8) && aligned(second, 4) && arena.used() == 24);
    }

    {
        Arena arena(256);
        void* first = arena.allocate(1, 1);
        void* second = arena.allocate(4, 8);
        report("padding", first != nullptr && aligned(second, 8) && arena.used() == 12);
    }

    {
        Arena arena(32);
        void* first = arena.allocate(24, 8);
        void* second = arena.allocate(16, 8);
        report("exhaustion", first != nullptr && second == nullptr && arena.used() == 24);
    }

    {
        Arena arena(32);
        void* zero = arena.allocate(0, 4);
        void* negative = arena.allocate(-3, 4);
        report("zero", zero == nullptr && negative == nullptr && arena.used() == 0);
    }

    {
        Arena arena(256);
        void* a = arena.allocate(3, 4);
        void* b = arena.allocate(5, 16);
        void* c = arena.allocate(2, 2);
        report("alignments", aligned(a, 4) && aligned(b, 16) && aligned(c, 2) &&
                                  arena.used() == 24);
    }

    {
        Arena arena(32);
        void* first = arena.allocate(32, 1);
        void* second = arena.allocate(1, 1);
        report("exact", first != nullptr && second == nullptr && arena.used() == 32);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
