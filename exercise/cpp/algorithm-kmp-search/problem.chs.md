这一题不读 stdin，也不写 stdout。实现一个函数，返回模式串在文本串中所有出现位置的起始下标。

```cpp
#include <cstddef>
#include <string>
#include <vector>

std::vector<std::size_t> find_all(const std::string& text,
                                  const std::string& pattern);
```

规则：

- 返回所有满足 `text[pos + i] == pattern[i]`（对所有合法的 `i`）的起始下标 `pos`，按升序排列。
- 允许重叠。例如在 `"aaaaa"` 中查找 `"aaa"`，结果是 `{0, 1, 2}`。
- 模式为空时返回空 `vector`。
- 模式比文本长时返回空 `vector`。
- 只包含 ASCII 字符，区分大小写。
- 请使用 KMP（前缀函数）实现；不要调用 `std::string::find` 反复搜索。
- 不要写 `main`，不要修改函数签名。
