#include <string>

class Shape {
public:
    virtual ~Shape() = default;
    virtual std::string label() const = 0;
    virtual double area() const = 0;
};

class Rect : public Shape {
public:
    Rect(double width, double height) : width_(width), height_(height) {}

    std::string label() const override { return "rect"; }

    double area() const override { return width_ * height_; }

private:
    double width_;
    double height_;
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}

    std::string label() const override { return "circle"; }

    double area() const override {
        constexpr double kPi = 3.14159265358979323846;
        return kPi * radius_ * radius_;
    }

private:
    double radius_;
};
