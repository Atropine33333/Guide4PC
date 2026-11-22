// Guide4PC harness appended to the student's code in the same translation unit.
#include <cmath>
#include <cstring>
#include <iostream>
#include <type_traits>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool closeTo(double actual, double expected) {
    return std::fabs(actual - expected) <= 1e-9 * (1.0 + std::fabs(expected));
}

constexpr double kHarnessPi = 3.14159265358979323846;

}  // namespace

int main() {
    {
        const bool ok = std::is_abstract<Shape>::value &&
                        std::has_virtual_destructor<Shape>::value &&
                        std::is_base_of<Shape, Rectangle>::value &&
                        std::is_base_of<Shape, Circle>::value;
        report("interface", ok);
    }

    {
        Rectangle rectangle(3.0, 4.0);
        int top = 0;
        (void)top;
        const bool ok = closeTo(rectangle.area(), 12.0) &&
                        std::strcmp(rectangle.name(), "rectangle") == 0;
        report("rectangle-area", ok);
    }

    {
        Circle circle(2.0);
        const bool ok = closeTo(circle.area(), kHarnessPi * 4.0) &&
                        std::strcmp(circle.name(), "circle") == 0;
        report("circle-area", ok);
    }

    {
        Rectangle rectangle(2.5, 4.0);
        Circle circle(1.5);
        Shape *shapes[2] = {&rectangle, &circle};
        const double sum = shapes[0]->area() + shapes[1]->area();
        const bool ok = closeTo(sum, 10.0 + kHarnessPi * 2.25) &&
                        std::strcmp(shapes[0]->name(), "rectangle") == 0 &&
                        std::strcmp(shapes[1]->name(), "circle") == 0;
        report("base-dispatch", ok);
    }

    {
        Rectangle flat(0.0, 5.0);
        Circle point(0.0);
        const bool ok = closeTo(flat.area(), 0.0) && closeTo(point.area(), 0.0);
        report("zero-dimensions", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
