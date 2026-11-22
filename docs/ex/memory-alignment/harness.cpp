// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>

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
    const std::size_t max = std::numeric_limits<std::size_t>::max();

    report("align-basic", align_up(0, 1) == 0 && align_up(5, 8) == 8 &&
                               align_up(8, 8) == 8 && align_up(9, 8) == 16 &&
                               align_up(1, 2) == 2 &&
                               align_up(12345, 4096) == 16384);

    report("align-zero", align_up(7, 0) == 7 && align_up(max, 0) == max);

    report("align-overflow", align_up(max, 2) == max &&
                                  align_up(max - 1, 2) == max - 1 &&
                                  align_up(max, 8) == max);

    {
        alignas(16) unsigned char buffer[32] = {};
        const void* aligned = buffer;
        const void* offset = buffer + 1;
        const bool ok = is_aligned(aligned, 16) && !is_aligned(offset, 16) &&
                        is_aligned(offset, 1) && is_aligned(nullptr, 8) &&
                        !is_aligned(aligned, 0) && !is_aligned(aligned, 3) &&
                        !is_aligned(nullptr, 3);
        report("is-aligned", ok);
    }

    report("padding", padding_for(0, 8) == 0 && padding_for(1, 8) == 7 &&
                           padding_for(8, 8) == 0 && padding_for(9, 8) == 7 &&
                           padding_for(7, 0) == 0 &&
                           padding_for(0, 1) == 0);

    {
        const std::size_t expected = (8 - max % 8) % 8;
        report("padding-max", padding_for(max, 8) == expected);
    }

    {
        bool ok = true;
        for (std::size_t value = 0; value < 200; ++value) {
            for (std::size_t alignment = 1; alignment <= 128; alignment *= 2) {
                const std::size_t aligned = align_up(value, alignment);
                if (aligned % alignment != 0 || aligned < value ||
                    aligned - value != padding_for(value, alignment)) {
                    ok = false;
                }
                const std::uintptr_t address =
                    reinterpret_cast<std::uintptr_t>(&value) + value;
                const void* pointer = reinterpret_cast<const void*>(address);
                if (is_aligned(pointer, alignment) !=
                    (address % alignment == 0)) {
                    ok = false;
                }
            }
        }
        report("consistency", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
