这一题不读 stdin。实现几个只观察、不拷贝字符的函数，非空返回值必须指回原字符串的缓冲区。

```cpp
#include <string>
#include <string_view>

std::string_view trim(std::string_view text);
std::string_view first_word(std::string_view text);
std::string_view between(std::string_view text, char open, char close);
bool starts_with(std::string_view text, std::string_view prefix);
```

约定：

- `trim` 去掉首尾的 ASCII 空格 `' '`，返回剩下的部分；全是空格或空串时返回空视图。
- `first_word` 返回从第一个非空格字符开始、到下一个空格或结尾为止的片段；没有非空格字符时返回空视图。
- `between` 返回第一个 `open` 之后、其后第一个 `close` 之前的片段；找不到 `open`，或 `open` 后面没有 `close` 时返回空视图。
- `starts_with` 判断 `prefix` 是否为 `text` 的前缀；空前缀视为真。
- 返回值必须直接指向传入的 `text`，不能指向局部对象或临时 `std::string`。
- 不要写 `main`。

### 提示

`std::string_view::substr` 返回的视图仍然指向原缓冲区；注意 `find` 返回 `std::string_view::npos` 的情况。
