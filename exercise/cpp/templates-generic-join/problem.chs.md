这一题不读 stdin，也不写 stdout。写一个适用于任何可遍历容器的拼接函数，测试在隐藏的 harness 里。

```cpp
#include <string>

template <typename Container>
std::string join(const Container& items, const std::string& separator);
```

规则：

- 用 `operator<<` 把每个元素写进结果，元素之间插入 `separator`。
- 第一个元素前面和最后一个元素后面都不加分隔符。
- 空容器返回空字符串。
- 容器可以是 `std::vector<int>`、`std::vector<std::string>`、`std::deque<int>` 等任何支持范围 `for` 的类型。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `join(std::vector<int>{1, 2, 3}, ", ")` | `1, 2, 3` |
| `join(std::vector<std::string>{"a", "b"}, "-")` | `a-b` |
| `join(std::vector<int>{}, ",")` | 空字符串 |
| `join(std::vector<int>{4, 5}, "")` | `45` |

### 提示

用 `std::ostringstream` 收集结果，`for (const auto& item : items)` 遍历。用一个 `bool first` 记录是不是第一个元素，避免在末尾多写一个分隔符。模板写在头文件里也无妨——本题所有代码都在同一个翻译单元里。
