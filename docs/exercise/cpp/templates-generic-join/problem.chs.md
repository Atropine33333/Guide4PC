门徒对耶稣说：「这是野地，众人散坐，饼和鱼都分完了，零碎还剩在地上。」

耶稣说：「把剩下的零碎收拾起来，免得有糟蹋的。」

门徒便取篮子来，一件一件收进去。收的时候，两件之间搁一道 `separator`；头一件前头不搁，末一件后头也不搁。

有人问：「篮子只收饼么？」

耶稣说：「不拘。整数收得，字句也收得，凡能一件一件取出来的，都收得。」

又有人问：「空篮子呢？」

耶稣说：「空篮子进来，空篮子出去。」

众人收拾完了，数一数，一件也没有糟蹋。

**人话**：写一个适用于任何可遍历容器的拼接函数 `join`，用 `operator<<` 把每个元素写进结果，元素之间插入 `separator`，首尾不加；空容器返回空字符串。

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

用 `std::ostringstream` 收集结果，`for (const auto& item : items)` 遍历。用一个 `bool first` 记录当前元素是否第一个，避免在末尾多写一个分隔符。模板写在头文件里也无妨——本题所有代码都在同一个翻译单元里。
