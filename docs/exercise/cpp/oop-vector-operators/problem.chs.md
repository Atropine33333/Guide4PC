这一题不读 stdin，也不写 stdout。实现一个带运算符的二维向量类型，测试在隐藏的 harness 里。

实现：

```cpp
struct Vec2 {
    double x;
    double y;

    Vec2(double x = 0.0, double y = 0.0);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(double scalar) const;
    Vec2 operator-() const;                  // 一元负号
    Vec2& operator+=(const Vec2& other);
    bool operator==(const Vec2& other) const;

    double dot(const Vec2& other) const;
    double length_squared() const;
};

Vec2 operator*(double scalar, const Vec2& value);  // 支持 2.0 * v
```

要求：

- 所有运算按分量逐个计算；
- 一元负号把两个分量都取反；
- `+=` 修改左操作数并返回它的引用；
- `==` 逐分量精确比较（本题的测试值都能被 `double` 精确表示）；
- `length_squared()` 返回 `x*x + y*y`，不要开平方；
- 不要写 `main`。
