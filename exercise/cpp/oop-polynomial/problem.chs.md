> 天元一术立为式，
> 自下而上列其筹。
> 高位空零须削去，
> 相加相乘自有由。

古有天元之术：以一为元，按次列筹，低次在前，高次在后，空位补零。

今有式若干，系数皆 64 位之数，自低次至高次列成一串。问：此式次数几何？取某次之系数几何？以某数入式，所得几何？又，二式相加、相乘，其式如何？

术曰：次数者，最高非零筹之位也；通为零者，记其次数为 `-1`。取系数，位在串外者报零。入式求值，自高次向下，逢筹乘元、遇位加筹，是谓霍纳。相加者，同位之筹相加；相乘者，逐位相攀而累之。所得皆削去高位之零，方为定式。

**人话**：实现系数为 `long long` 的稠密多项式类 `Polynomial`：构造参数按次数从低到高排列，下标 `i` 是 `x^i` 的系数；零多项式次数为 `-1`，非零者取最高非零系数的下标；构造与运算结果都要去掉高位零，所以 `{1, 2, 0}` 与 `{1, 2}` 是同一个式子；`coefficient` 对越界次数返回 0；`evaluate` 用霍纳法求值；`+`、`*` 返回规范化后的新对象；`==` 按数值比较。

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

### 规则

- 构造函数的 `coefficients` 按次数从低到高排列：下标 `i` 是 `x^i` 的系数。
- 零多项式的次数为 `-1`；非零多项式的次数是最高非零系数的下标。
- 构造和运算结果都要去掉高位零系数，因此 `{1, 2, 0}` 与 `{1, 2}` 表示同一个多项式。
- `coefficient(power)` 对负数或超出范围的次数返回 0。
- `evaluate(x)` 使用 Horner 法或等价方式求值。
- `operator+` 和 `operator*` 返回规范化后的新对象；零多项式参与运算不能出错。
- `operator==` 按数值比较，忽略高位零系数。
- 不要写 `main`，不要修改公开接口。
