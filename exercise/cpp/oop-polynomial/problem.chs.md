这一题不读 stdin，也不写 stdout。实现一个系数为 64 位整数的稠密多项式类。

```cpp
#include <vector>

class Polynomial {
public:
    Polynomial();
    explicit Polynomial(std::vector<long long> coefficients);

    int degree() const;
    long long coefficient(int power) const;
    long long evaluate(long long x) const;

    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator*(const Polynomial& other) const;
    bool operator==(const Polynomial& other) const;
};
```

规则：

- 构造函数的 `coefficients` 按次数从低到高排列：下标 `i` 是 `x^i` 的系数。
- 零多项式的次数为 `-1`；非零多项式的次数是最高非零系数的下标。
- 构造和运算结果都要去掉高位零系数，因此 `{1, 2, 0}` 与 `{1, 2}` 表示同一个多项式。
- `coefficient(power)` 对负数或超出范围的次数返回 0。
- `evaluate(x)` 使用 Horner 法或等价方式求值。
- `operator+` 和 `operator*` 返回规范化后的新对象；零多项式参与运算不能出错。
- `operator==` 按数值比较，忽略高位零系数。
- 不要写 `main`，不要修改公开接口。
