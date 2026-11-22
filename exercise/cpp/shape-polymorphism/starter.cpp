class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual const char* name() const = 0;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}

    double area() const override {
        // TODO: 返回 width * height。
        return 0.0;
    }

    const char* name() const override {
        // TODO: 返回 "rectangle"。
        return "shape";
    }

private:
    double width_ = 0.0;
    double height_ = 0.0;
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}

    double area() const override {
        // TODO: 使用 3.14159265358979323846 计算面积。
        return 0.0;
    }

    const char* name() const override {
        // TODO: 返回 "circle"。
        return "shape";
    }

private:
    double radius_ = 0.0;
};
