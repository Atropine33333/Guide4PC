// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <cstdint>
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

}  // namespace

int main() {
    {
        HandleStore store;
        const Handle a = store.create(10);
        const Handle b = store.create(20);
        const Handle c = store.create(30);
        report("basic", store.size() == 3 &&
                            store.valid(a) && store.valid(b) &&
                            store.valid(c) && store.get(a) != nullptr &&
                            *store.get(a) == 10 && *store.get(b) == 20 &&
                            *store.get(c) == 30 &&
                            store.capacity() == HandleStore::kCapacity);
    }

    {
        HandleStore store;
        const Handle old = store.create(5);
        const std::uint32_t index = old.index;
        const std::uint32_t generation = old.generation;
        const bool destroyed = store.destroy(old);
        const bool invalid_after = !store.valid(old) && store.get(old) == nullptr;
        const Handle fresh = store.create(9);
        report("stale", destroyed && invalid_after && fresh.index == index &&
                            fresh.generation != generation &&
                            old.generation == generation &&
                            !store.valid(old) && store.get(old) == nullptr &&
                            store.valid(fresh) && *store.get(fresh) == 9 &&
                            store.size() == 1);
    }

    {
        HandleStore store;
        const Handle missing{HandleStore::kInvalidIndex, 0};
        const Handle out_of_range{HandleStore::kCapacity, 1};
        const Handle zero_generation{0, 0};
        report("invalid", !store.valid(missing) && !store.valid(out_of_range) &&
                              !store.valid(zero_generation) &&
                              store.get(missing) == nullptr &&
                              store.get(out_of_range) == nullptr &&
                              store.get(zero_generation) == nullptr &&
                              !store.destroy(missing) && store.size() == 0);
    }

    {
        HandleStore store;
        const Handle handle = store.create(42);
        const bool first = store.destroy(handle);
        const bool second = store.destroy(handle);
        report("double", first && !second && !store.valid(handle) &&
                             store.get(handle) == nullptr && store.size() == 0);
    }

    {
        HandleStore store;
        Handle handles[HandleStore::kCapacity];
        bool filled = true;
        for (std::uint32_t i = 0; i < HandleStore::kCapacity; ++i) {
            handles[i] = store.create(static_cast<int>(i));
            if (!store.valid(handles[i])) filled = false;
        }
        const Handle overflow = store.create(99);
        bool destroyed = true;
        for (std::uint32_t i = 0; i < HandleStore::kCapacity; ++i) {
            if (!store.destroy(handles[i])) destroyed = false;
        }
        const Handle reused = store.create(7);
        report("capacity", filled && overflow.index == HandleStore::kInvalidIndex &&
                               overflow.generation == 0 && destroyed &&
                               store.size() == 1 && store.valid(reused) &&
                               *store.get(reused) == 7);
    }

    {
        HandleStore store;
        const Handle handle = store.create(123);
        const HandleStore& view = store;
        const int* value = view.get(handle);
        report("const", value != nullptr && *value == 123 &&
                            view.valid(handle));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
