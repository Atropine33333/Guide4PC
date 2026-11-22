// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

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
        BloomFilter filter(1024, 3);
        const bool initially_empty = !filter.maybe_contains("apple") &&
                                     filter.set_bits() == 0 &&
                                     filter.bit_count() == 1024;
        filter.add("apple");
        filter.add("banana");
        report("basic", initially_empty && filter.maybe_contains("apple") &&
                            filter.maybe_contains("banana") &&
                            filter.set_bits() >= 2 &&
                            filter.set_bits() <= 1024);
    }

    {
        BloomFilter filter(4096, 4);
        bool inserted = true;
        for (int i = 0; i < 200; ++i) {
            const std::string value = "key-" + std::to_string(i * 17);
            filter.add(value);
        }
        for (int i = 0; i < 200; ++i) {
            const std::string value = "key-" + std::to_string(i * 17);
            if (!filter.maybe_contains(value)) inserted = false;
        }
        report("no-false-negative", inserted && filter.set_bits() <= 4096);
    }

    {
        BloomFilter filter(512, 3);
        filter.add("stable");
        const std::size_t once = filter.set_bits();
        for (int i = 0; i < 5; ++i) filter.add("stable");
        report("duplicate", filter.set_bits() == once &&
                                filter.maybe_contains("stable"));
    }

    {
        BloomFilter one(1, 5);
        one.add("x");
        BloomFilter few(4, 2);
        few.add("alpha");
        report("tiny", one.bit_count() == 1 && one.set_bits() == 1 &&
                           one.maybe_contains("x") && few.bit_count() == 4 &&
                           few.set_bits() >= 1 && few.set_bits() <= 4);
    }

    {
        std::string long_value(1000, 'z');
        BloomFilter filter(2048, 3);
        filter.add("");
        filter.add(long_value);
        report("empty-string", filter.maybe_contains("") &&
                                   filter.maybe_contains(long_value));
    }

    {
        BloomFilter filter(8, 2);
        bool all_found = true;
        for (int i = 0; i < 100; ++i) {
            const std::string value = "dense-" + std::to_string(i);
            filter.add(value);
        }
        for (int i = 0; i < 100; ++i) {
            const std::string value = "dense-" + std::to_string(i);
            if (!filter.maybe_contains(value)) all_found = false;
        }
        report("dense", all_found && filter.set_bits() <= 8 &&
                            filter.bit_count() == 8);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
