实现一个把“名称 + 金额”排成定宽表格行的函数。

```cpp
#include <ostream>
#include <string>

std::string format_row(const std::string& name, long long amount);
void print_row(std::ostream& out, const std::string& name, long long amount);
```

规则：

- `format_row` 返回一行文本，不含换行符。
- 名称左对齐，占 12 个字符；不足 12 个时在右侧补空格，超过 12 个时原样输出、不截断。
- 名称之后紧跟一个 `|`。
- 金额右对齐，占 10 个字符；不足 10 个时在左侧补空格，超过 10 个时原样输出。
- `print_row` 把 `format_row` 的结果和一个 `'\n'` 写到 `out`，不要用 `std::cout`。
- 不要写 `main`。

### 示例

- `format_row("ann", 5)` 返回 `ann         |         5`
- `format_row("abcdefghijkl", 1234567890)` 返回 `abcdefghijkl|1234567890`
- `format_row("bob", -42)` 返回 `bob         |       -42`

### 提示

`std::setw(12) << std::left` 管名称，`std::setw(10) << std::right` 管金额；宽度只是下限，内容更长时不会被截断。`std::setfill(' ')` 用来确保填充的是空格。
