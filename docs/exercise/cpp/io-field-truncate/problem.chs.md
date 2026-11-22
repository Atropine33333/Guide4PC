实现两个把文本放进固定宽度字段的函数。

```cpp
#include <cstddef>
#include <string>

std::string truncate_field(const std::string& text, std::size_t width);

std::string center_field(const std::string& text, std::size_t width);
```

规则：

- `truncate_field`：
  - 若 `text.size() <= width`，返回原字符串，不补空格；
  - 若 `text.size() > width`，只保留前 `width` 个字符；
  - `width == 0` 时返回空字符串。
- `center_field`：
  - 若 `text.size() >= width`，返回 `truncate_field(text, width)` 的结果；
  - 否则在左右两边补空格，使结果长度恰好为 `width`；
  - 两边空格数相差最多 1；当补空格总数为奇数时，右边比左边多一个空格（例如补空格总数为 3 时，左边 1 个、右边 2 个）。
- 输入文本只包含可打印 ASCII 字符，不需要考虑 UTF-8 或显示宽度。
- 不要写 `main`，不要修改函数签名。
