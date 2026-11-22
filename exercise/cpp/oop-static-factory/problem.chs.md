这一题不读 stdin，也不写 stdout。实现一个只能通过静态工厂创建的用户名类，测试在隐藏的 harness 里。

```cpp
#include <optional>
#include <string>
#include <utility>

class Username {
public:
    static std::optional<Username> create(const std::string& text);
    const std::string& value() const;
    std::size_t length() const;
    bool operator==(const Username& other) const;

private:
    explicit Username(std::string text);
    std::string text_;
};
```

规则：

- 合法用户名长度必须在 3 到 16 之间（含两端）。
- 第一个字符必须是 ASCII 字母（`a`–`z` 或 `A`–`Z`）。
- 其余字符只能是 ASCII 字母、数字或下划线。
- 不合法时 `create` 返回 `std::nullopt`；合法时返回包含用户名的 `std::optional`。
- `value()` 返回保存的用户名，`length()` 返回它的长度。
- `operator==` 比较两个用户名的字符串内容。
- 构造函数是私有的，外部只能通过 `create` 创建对象。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `Username::create("alice_01")` | 有值，`length()` 为 8 |
| `Username::create("ab")` | `std::nullopt` |
| `Username::create("1alice")` | `std::nullopt` |
| `Username::create("alice!")` | `std::nullopt` |

### 提示

先检查长度，再检查首字符，最后遍历其余字符。`std::optional` 可以直接由 `Username` 构造；因为 `create` 是类的成员，它能访问私有构造函数。注意 `std::isalpha` 和 `std::isdigit` 对负的 `char` 有未定义行为，可以只比较 ASCII 区间，或先把字符转成 `unsigned char`。
