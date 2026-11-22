// Guide4PC harness appended to the student's code in the same translation unit.
#include <cmath>
#include <iostream>
#include <memory>
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

bool near(double a, double b) { return std::fabs(a - b) < 1e-9; }

class Probe : public Shape {
public:
    Probe() { ++alive; }
    ~Probe() { --alive; }
    std::string label() const override { return "probe"; }
    double area() const override { return 0.0; }
    static int alive;
};

int Probe::alive = 0;

}  // namespace

int main() {
    {
        const Rect rect(2.0, 3.0);
        report("rect-basic", rect.label() == "rect" && near(rect.area(), 6.0));
    }

    {
        const Circle circle(2.0);
        const double pi = 3.14159265358979323846;
        report("circle-basic",
               circle.label() == "circle" && near(circle.area(), pi * 4.0));
    }

    {
        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::unique_ptr<Shape>(new Rect(2.0, 3.0)));
        shapes.push_back(std::unique_ptr<Shape>(new Circle(1.0)));
        shapes.push_back(std::unique_ptr<Shape>(new Rect(0.0, 5.0)));
        double total = 0.0;
        bool labels = shapes[0]->label() == "rect" &&
                      shapes[1]->label() == "circle";
        for (const std::unique_ptr<Shape>& shape : shapes) {
            total += shape->area();
        }
        report("polymorphic", labels && near(total, 6.0 + 3.14159265358979323846));
    }

    {
        Shape* base = new Probe();
        const bool created = Probe::alive == 1;
        delete base;
        report("virtual-destructor", created && Probe::alive == 0);
    }

    {
        const Rect empty_rect(0.0, 5.0);
        const Circle empty_circle(0.0);
        report("zero-size", near(empty_rect.area(), 0.0) &&
                                near(empty_circle.area(), 0.0) &&
                                empty_rect.label() == "rect" &&
                                empty_circle.label() == "circle");
    }

    {
        std::vector<Shape*> shapes;
        double total = 0.0;
        for (int i = 0; i < 100; ++i) {
            shapes.push_back(new Rect(1.0, 1.0));
        }
        for (Shape* shape : shapes) {
            total += shape->area();
            delete shape;
        }
        report("many-objects", near(total, 100.0));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
