实现一个以“分”为单位的金额类，并让它支持流式输入输出。补全下面的接口：

```cpp
class Money {
public:
    Money() = default;
    explicit Money(long long cents);

    long long cents() const;

    Money operator+(const Money& other) const;
    Money operator-(const Money& other) const;
    Money operator*(int factor) const;

    bool operator==(const Money& other) const;
    bool operator<(const Money& other) const;

private:
    long long cents_ = 0;
};

std::ostream& operator<<(std::ostream& out, const Money& money);
std::istream& operator>>(std::istream& in, Money& money);
```

输出格式固定为美元符号加两位小数：`1234` 分输出 `$12.34`，`5` 分输出 `$0.05`，`-5` 分输出 `-$0.05`，`0` 分输出 `$0.00`。

输入是一个空白分隔的记号，语法为：可选 `-`，可选 `$`，至少一位数字，可选 `.` 加一或两位小数。例如 `12.34`、`-$0.05`、`$3.25`、`7` 都合法；`0.5` 表示 50 分。非法记号（如 `abc`、`1.234`）必须把流的 `failbit` 置位，并且不修改 `money` 的当前值。

评测代码会直接包含你的实现，并使用 `std::istringstream` / `std::ostringstream` 调用这些运算符。**不要写 `main`。**

### 输入

无。

### 输出

无。

### 示例

```cpp
Money price;
std::istringstream("$3.25") >> price;   // price.cents() == 325
std::ostringstream out;
out << price;                            // "$3.25"
```

### 约束

- 金额绝对值不超过 `10^15` 分。
- 不使用 `printf`/`scanf`，只使用 `iostream`。
