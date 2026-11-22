> 方田有术分之于母，
> 子母相推约其数。
> 减而更减求其等，
> 等数除之乃成故。

《九章·方田》有约分之术：以少减多，更相减损，求其等也，以等数约之。

今有分数，分子曰子，分母曰母，母不为零。问：约之至简，当作何状？二分数相加、相减、相乘、相除，各得何数？又，二数孰大孰小，相等否？

术曰：母者必正，子母互质，方为最简。母负者，子母同变其号；子为零者，一以当之。加减者，先通分而后并其子；乘者，子乘子、母乘母；除者，颠倒相乘。毕则仍以等数约之。比大小者，交叉相乘，视其积，毋用小数。

**人话**：实现有理数类 `Rational`：构造后即约成最简，分母恒正、子母互质，`0` 记作 `0/1`，`2/-4` 记作 `-1/2`；四则运算的结果同样最简；`==` 与 `<` 都按数值比较，`<` 用交叉相乘，不借浮点。约分用 `std::gcd`。

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

### 要求

- 每个有理数都保存为最简形式：分母始终为正，分子与分母互质；
- `0` 统一表示为 `0/1`；输入的分母可能为负，例如 `2/-4` 应表示为 `-1/2`；
- 四则运算的结果也必须是最简形式；
- `==` 按数值比较，因此 `2/4` 与 `1/2` 相等；
- `<` 按数值比较，不要用浮点数，以免精度问题；
- 不要写 `main`。

提示：用 `std::gcd` 约分；比较 `a/b < c/d` 可以交叉相乘为 `a*d < c*b`（分母都为正）。
