这一题不读 stdin，也不写 stdout。实现 ISO 日期的严格解析与短格式输出。

```cpp
#include <string>

struct Date {
    int year;
    int month;
    int day;
};

bool parse_iso_date(const std::string& text, Date& out);

std::string format_short_date(const Date& date);
```

规则：

- `parse_iso_date` 只接受严格形状 `YYYY-MM-DD`：
  - 长度必须正好是 10；
  - 第 5 个和第 8 个字符必须是 `-`；
  - 其余位置必须都是 ASCII 数字；
  - 年份在 1 到 9999 之间，月份在 1 到 12 之间，日期必须真实存在。
- 闰年规则：能被 4 整除，但不能被 100 整除的年份是闰年；能被 400 整除的年份也是闰年。二月在闰年有 29 天，平年 28 天。
- 解析成功时把年月日写入 `out` 并返回 `true`。失败时返回 `false`，并且**不要修改** `out`。
- `format_short_date` 返回 `DD/MM/YYYY`：日、月各两位，不足补 `0`，年份四位；例如 `2024-02-09` 变成 `09/02/2024`。可以假定传入的 `date` 已经合法。
- 不要写 `main`，不要修改结构体和函数签名。
