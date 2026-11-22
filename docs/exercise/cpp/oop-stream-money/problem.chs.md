“哦，我亲爱的朋友，您总算来了！”账房先生把眼镜往上推了推，“这笔钱哪，一律按‘分’记，一个子儿也不能含糊。”

“写出去的时候，”他压低声音，“先画个美元符号，再写两位小数。一千二百三十四分，写成 `$12.34`；五分，写成 `$0.05`；欠人五分，前头加个负号，`-$0.05`；一分没有，`$0.00`。老天爷，少一个零都不行。”

“读进来的时候更得留神：前头兴许有个负号，兴许有个美元符号，后头兴许带小数点，点后一位或两位。‘`12.34`’、‘`-$0.05`’、‘`$3.25`’、‘`7`’都成；‘`0.5`’算五十分。要是有人递来‘`abc`’或者‘`1.234`’——哦，那可不行，把流的 `failbit` 立起来，钱数一个子儿也别动！”

**人话**：实现以“分”为单位的金额类 `Money`：`+`、`-`、`*` 按分运算，`==`、`<` 比较金额；`operator<<` 按美元符号加两位小数输出，`operator>>` 按“可选的负号、可选的 `$`、至少一位数字、可选的一到两位小数”读入，非法记号置 `failbit` 且不改动对象。

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
