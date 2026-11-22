struct Vec2 {
    double x;
    double y;

    Vec2(double x_ = 0.0, double y_ = 0.0) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& other) const {
        // TODO: 逐分量相加。
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator-(const Vec2& other) const {
        // TODO: 逐分量相减。
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator*(double scalar) const {
        // TODO: 逐分量乘以 scalar。
        (void)scalar;
        return *this;
    }

    Vec2 operator-() const {
        // TODO: 两个分量都取反。
        return *this;
    }

    Vec2& operator+=(const Vec2& other) {
        // TODO: 修改自身。
        (void)other;
        return *this;
    }

    bool operator==(const Vec2& other) const {
        // TODO: 逐分量比较。
        (void)other;
        return true;
    }

    double dot(const Vec2& other) const {
        // TODO: 返回点积。
        (void)other;
        return 0.0;
    }

    double length_squared() const {
        // TODO: 返回 x*x + y*y。
        return 0.0;
    }
};

Vec2 operator*(double scalar, const Vec2& value) {
    // TODO: 复用成员 operator*。
    (void)scalar;
    return value;
}
