// 参考实现：oop-vector-operators
struct Vec2 {
    double x;
    double y;

    Vec2(double x_ = 0.0, double y_ = 0.0) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator*(double scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 operator-() const {
        return Vec2(-x, -y);
    }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }

    double dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    double length_squared() const {
        return x * x + y * y;
    }
};

Vec2 operator*(double scalar, const Vec2& value) {
    return value * scalar;
}
