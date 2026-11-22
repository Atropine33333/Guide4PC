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

int h_normalize(int count) {
    int n = count % 32;
    if (n < 0) n += 32;
    return n;
}

std::uint32_t h_rotl(std::uint32_t value, int count) {
    const int n = h_normalize(count);
    if (n == 0) return value;
    return (value << n) | (value >> (32 - n));
}

std::uint32_t h_rotr(std::uint32_t value, int count) {
    const int n = h_normalize(count);
    if (n == 0) return value;
    return (value >> n) | (value << (32 - n));
}

std::uint32_t h_reverse(std::uint32_t value) {
    std::uint32_t result = 0;
    for (int i = 0; i < 32; ++i) {
        result = (result << 1) | (value & 1u);
        value >>= 1;
    }
    return result;
}

}  // namespace

int main() {
    report("left-basic", rotate_left(0x80000001u, 1) == 0x00000003u &&
                             rotate_left(0x12345678u, 8) == 0x34567812u &&
                             rotate_left(1u, 31) == 0x80000000u &&
                             rotate_left(0xFFFFFFFFu, 5) == 0xFFFFFFFFu);

    report("right-basic", rotate_right(0x12345678u, 8) == 0x78123456u &&
                              rotate_right(1u, 1) == 0x80000000u &&
                              rotate_right(0x80000000u, 1) == 0x40000000u);

    report("counts", rotate_left(0x12345678u, 0) == 0x12345678u &&
                         rotate_left(0x12345678u, 32) == 0x12345678u &&
                         rotate_left(0x12345678u, 33) == 0x2468ACF0u &&
                         rotate_left(0x12345678u, -1) == rotate_right(0x12345678u, 1) &&
                         rotate_right(0x12345678u, 40) == rotate_right(0x12345678u, 8) &&
                         rotate_right(0x12345678u, 64) == 0x12345678u &&
                         rotate_left(0x12345678u, -3) == 0x02468ACFu);

    {
        const std::uint32_t values[] = {0u, 1u, 0x80000000u, 0x12345678u,
                                        0xDEADBEEFu, 0xAAAAAAAAu};
        const int counts[] = {-100, -33, -1, 0, 1, 7, 31, 32, 65};
        bool ok = true;
        for (std::uint32_t value : values) {
            for (int count : counts) {
                if (rotate_right(rotate_left(value, count), count) != value) ok = false;
                if (rotate_left(rotate_right(value, count), count) != value) ok = false;
            }
        }
        report("inverse", ok);
    }

    report("reverse-known", reverse_bits(0x12345678u) == 0x1E6A2C48u &&
                                reverse_bits(0x0000FFFFu) == 0xFFFF0000u &&
                                reverse_bits(1u) == 0x80000000u &&
                                reverse_bits(0u) == 0u &&
                                reverse_bits(0xFFFFFFFFu) == 0xFFFFFFFFu &&
                                reverse_bits(0x80000001u) == 0x80000001u);

    {
        std::uint32_t state = 20240517u;
        bool ok = true;
        for (int i = 0; i < 256; ++i) {
            state = state * 1664525u + 1013904223u;
            if (reverse_bits(state) != h_reverse(state)) ok = false;
            if (reverse_bits(reverse_bits(state)) != state) ok = false;
        }
        report("reverse-involution", ok);
    }

    {
        std::uint32_t state = 998244353u;
        const int counts[] = {-37, -1, 0, 1, 5, 16, 31, 32, 33, 64};
        bool ok = true;
        for (int i = 0; i < 256; ++i) {
            state = state * 1664525u + 1013904223u;
            for (int count : counts) {
                if (rotate_left(state, count) != h_rotl(state, count)) ok = false;
                if (rotate_right(state, count) != h_rotr(state, count)) ok = false;
            }
        }
        report("rotate-random", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
