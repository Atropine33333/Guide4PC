// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
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
        Pixel pixel(12, 34, 56);
        report("construction", pixel.red() == 12 && pixel.green() == 34 &&
                                   pixel.blue() == 56);
    }

    {
        Pixel pixel(300, 256, 999);
        report("clamp-high", pixel.red() == 255 && pixel.green() == 255 &&
                                 pixel.blue() == 255);
    }

    {
        Pixel pixel(-1, -20, -300);
        report("clamp-low", pixel.red() == 0 && pixel.green() == 0 &&
                                pixel.blue() == 0 && pixel.is_black());
    }

    {
        Pixel pixel(1, 2, 3);
        pixel.set_red(300);
        pixel.set_green(-5);
        pixel.set_blue(128);
        report("setters", pixel.red() == 255 && pixel.green() == 0 &&
                              pixel.blue() == 128);
    }

    {
        Pixel low(10, 20, 31);
        Pixel tiny(0, 0, 1);
        Pixel full(255, 255, 255);
        report("brightness", low.brightness() == 20 && tiny.brightness() == 0 &&
                                  full.brightness() == 255);
    }

    {
        Pixel black(0, 0, 0);
        Pixel almost(0, 0, 1);
        Pixel gray(1, 1, 1);
        report("black", black.is_black() && !almost.is_black() && !gray.is_black());
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
