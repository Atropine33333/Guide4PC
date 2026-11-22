// 参考实现：shape-polymorphism
namespace {
constexpr double kPi = 3.14159265358979323846;
}  // namespace

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual const char* name() const = 0;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}

    double area() const override { return width_ * height_; }

    const char* name() const override { return "rectangle"; }

private:
    double width_ = 0.0;
    double height_ = 0.0;
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}

    double area() const override { return kPi * radius_ * radius_; }

    const char* name() const override { return "circle"; }

private:
    double radius_ = 0.0;
};
