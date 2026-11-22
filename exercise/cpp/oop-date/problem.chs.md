实现一个日期类。

```cpp
class Date {
public:
    Date(int year, int month, int day);

    bool valid() const;
    void next_day();

    int year() const;
    int month() const;
    int day() const;

    static bool is_leap(int year);
    static int days_in_month(int year, int month);

    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
};
```

规则：

- 合法日期：年份 `1..9999`，月份 `1..12`，日期 `1..days_in_month(year, month)`。
- `is_leap(y)`：能被 4 整除但不能被 100 整除，或者能被 400 整除。
- `days_in_month(y, m)`：月份非法时返回 0；二月根据闰年返回 29 或 28；其余月份返回 30 或 31。
- `valid()` 判断当前对象；非法对象保留构造时的三个值。
- `next_day()` 把日期推进一天；非法日期不变；`9999-12-31` 的下一天超出年份范围，也不变。
- `<` 和 `==` 按年、月、日依次做字典序比较，不要求日期合法。
- 不要写 `main`。

### 提示

`next_day` 先把 `day` 加 1，若超过当月天数则把 `day` 置 1、月份加 1；若月份超过 12 则把月份置 1、年份加 1。`9999-12-31` 要单独处理。
