#include <string>

class Shape {
public:
    ~Shape() = default;  // TODO: 改成虚析构函数
    virtual std::string label() const = 0;
    virtual double area() const = 0;
};

class Rect : public Shape {
public:
    Rect(double width, double height) : width_(width), height_(height) {}

    std::string label() const override {
        return {};  // TODO
    }

    double area() const override {
        return 0.0 * width_ * height_;  // TODO
    }

private:
    double width_;
    double height_;
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}

    std::string label() const override {
        return {};  // TODO
    }

    double area() const override {
        return 0.0 * radius_;  // TODO
    }

private:
    double radius_;
};
