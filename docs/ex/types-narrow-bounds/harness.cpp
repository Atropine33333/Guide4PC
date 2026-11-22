// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>

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
        std::int8_t out = 0;
        bool ok = narrow_i8(127, out) && out == 127;
        ok = ok && narrow_i8(-128, out) && out == -128;
        ok = ok && !narrow_i8(128, out) && !narrow_i8(-129, out);
        report("i8-bounds", ok);
    }

    {
        std::int16_t out = 0;
        bool ok = narrow_i16(32767, out) && out == 32767;
        ok = ok && narrow_i16(-32768, out) && out == -32768;
        ok = ok && !narrow_i16(32768, out) && !narrow_i16(-32769, out);
        report("i16-bounds", ok);
    }

    {
        std::uint8_t out = 0;
        bool ok = narrow_u8(0, out) && out == 0;
        ok = ok && narrow_u8(255, out) && out == 255;
        ok = ok && !narrow_u8(256, out) && !narrow_u8(-1, out);
        report("u8-bounds", ok);
    }

    {
        std::int32_t out = 0;
        bool ok = narrow_i32(2147483647LL, out) && out == 2147483647;
        ok = ok && narrow_i32(-2147483648LL, out) && out == -2147483648LL;
        ok = ok && !narrow_i32(2147483648LL, out) && !narrow_i32(-2147483649LL, out);
        report("i32-bounds", ok);
    }

    {
        std::int8_t small = 42;
        std::int16_t medium = 42;
        std::uint8_t byte = 42;
        std::int32_t large = 42;
        const bool ok = !narrow_i8(128, small) && small == 42 &&
                        !narrow_i16(32768, medium) && medium == 42 &&
                        !narrow_u8(256, byte) && byte == 42 &&
                        !narrow_i32(2147483648LL, large) && large == 42;
        report("unchanged-on-failure", ok);
    }

    {
        std::mt19937 rng(20241123u);
        bool ok = true;
        for (int step = 0; step < 300 && ok; ++step) {
            const long long value = static_cast<long long>(rng() % 200001u) - 100000;
            std::int8_t a = 0;
            std::int16_t b = 0;
            std::uint8_t c = 0;
            std::int32_t d = 0;
            const bool in8 = value >= -128 && value <= 127;
            const bool in16 = value >= -32768 && value <= 32767;
            const bool inu8 = value >= 0 && value <= 255;
            const bool in32 = value >= -2147483648LL && value <= 2147483647LL;
            ok = ok && narrow_i8(value, a) == in8 && (!in8 || a == value);
            ok = ok && narrow_i16(value, b) == in16 && (!in16 || b == value);
            ok = ok && narrow_u8(value, c) == inu8 && (!inu8 || c == value);
            ok = ok && narrow_i32(value, d) == in32 && (!in32 || d == value);
        }
        report("random-values", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
