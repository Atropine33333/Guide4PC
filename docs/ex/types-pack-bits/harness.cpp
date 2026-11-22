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

}  // namespace

int main() {
    report("pack-basic", pack_rgba(1, 2, 3, 4) == 0x04010203u &&
                             pack_rgba(255, 0, 128, 16) == 0x10FF0080u &&
                             pack_rgba(0, 0, 0, 1) == 0x01000000u);

    report("pack-clamp", pack_rgba(-5, 300, 0, 999) == 0xFF00FF00u &&
                             pack_rgba(256, 256, 256, 256) == 0xFFFFFFFFu &&
                             pack_rgba(-1, -1, -1, -1) == 0u);

    report("pack-zero", pack_rgba(0, 0, 0, 0) == 0u);

    {
        int r = -1, g = -1, b = -1, a = -1;
        unpack_rgba(0x0A0B0C0Du, r, g, b, a);
        const bool one = r == 11 && g == 12 && b == 13 && a == 10;
        unpack_rgba(0xDEADBEEFu, r, g, b, a);
        report("unpack-basic", one && r == 0xAD && g == 0xBE && b == 0xEF && a == 0xDE);
    }

    {
        int r = 0, g = 0, b = 0, a = 0;
        unpack_rgba(0u, r, g, b, a);
        const bool zeros = r == 0 && g == 0 && b == 0 && a == 0;
        unpack_rgba(0xFFFFFFFFu, r, g, b, a);
        report("unpack-masks", zeros && r == 255 && g == 255 && b == 255 && a == 255);
    }

    {
        const int channels[][4] = {{0, 0, 0, 0},   {255, 255, 255, 255},
                                   {1, 2, 3, 4},   {17, 34, 51, 68},
                                   {200, 100, 50, 25}};
        bool ok = true;
        for (const auto &c : channels) {
            int r = -1, g = -1, b = -1, a = -1;
            unpack_rgba(pack_rgba(c[0], c[1], c[2], c[3]), r, g, b, a);
            if (r != c[0] || g != c[1] || b != c[2] || a != c[3]) ok = false;
        }
        report("round-trip", ok);
    }

    {
        int r = 0, g = 0, b = 0, a = 0;
        unpack_rgba(swap_rb(0x11223344u), r, g, b, a);
        report("swap-basic", r == 0x44 && g == 0x33 && b == 0x22 && a == 0x11 &&
                                 swap_rb(0x11223344u) == 0x11443322u);
    }

    {
        const std::uint32_t values[] = {0u, 0xFFFFFFFFu, 0x11223344u, 0xDEADBEEFu,
                                        0x000000FFu, 0xFF000000u};
        bool ok = true;
        for (std::uint32_t value : values) {
            if (swap_rb(swap_rb(value)) != value) ok = false;
            if ((swap_rb(value) & 0xFF00FF00u) != (value & 0xFF00FF00u)) ok = false;
        }
        report("swap-involution", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
