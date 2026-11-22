这一题不读 stdin，也不写 stdout。实现一个有理数类，测试在隐藏的 harness 里。

实现：

```cpp
class Rational {
public:
    Rational(int numerator, int denominator);  // denominator != 0

    int numerator() const;
    int denominator() const;

    Rational operator+(const Rational& other) const;
    Rational operator-(const Rational& other) const;
    Rational operator*(const Rational& other) const;
    Rational operator/(const Rational& other) const;  // other 不为 0

    bool operator==(const Rational& other) const;
    bool operator<(const Rational& other) const;
};
```

要求：

- 每个有理数都保存为最简形式：分母始终为正，分子与分母互质；
- `0` 统一表示为 `0/1`；输入的分母可能为负，例如 `2/-4` 应表示为 `-1/2`；
- 四则运算的结果也必须是最简形式；
- `==` 按数值比较，因此 `2/4` 与 `1/2` 相等；
- `<` 按数值比较，不要用浮点数，以免精度问题；
- 不要写 `main`。

提示：用 `std::gcd` 约分；比较 `a/b < c/d` 可以交叉相乘为 `a*d < c*b`（分母都为正）。
