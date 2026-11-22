Guide4PC 练习司文件

练发〔2026〕9 号

关于印发《用户名登记管理办法》的通知

各实现单位：

　　为规范用户名之登记，现将《用户名登记管理办法》印发给你们，请遵照执行。

　　第一条 用户名长度须在 3 至 16 之间（含两端）。

　　第二条 首字须为 ASCII 字母，即 `a`–`z` 或 `A`–`Z`。

　　第三条 其余各字，限 ASCII 字母、数字、下划线。

　　第四条 不合本办法者，`create` 报 `std::nullopt`；合者，报出含该名的 `std::optional`。

　　第五条 构造函数列为私有，外部不得径行创建，只可经 `create` 登记。

　　第六条 `value()` 报所存之名，`length()` 报其字数，`operator==` 比其内容。

　　第七条 本办法自发布之日起施行。

**人话**：实现只能由静态工厂创建的用户名类 `Username`：`create` 先查长度 3 到 16，再查首字是 ASCII 字母，最后逐个查其余字符；合法时返回装着用户名的 `std::optional`，不合法返回 `std::nullopt`。构造函数私有，`value()` 与 `length()` 读取内容，`operator==` 比较字符串。

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

### 示例

| 调用 | 结果 |
| --- | --- |
| `Username::create("alice_01")` | 有值，`length()` 为 8 |
| `Username::create("ab")` | `std::nullopt` |
| `Username::create("1alice")` | `std::nullopt` |
| `Username::create("alice!")` | `std::nullopt` |

### 提示

先检查长度，再检查首字符，最后遍历其余字符。`std::optional` 可以直接由 `Username` 构造；因为 `create` 是类的成员，它能访问私有构造函数。注意 `std::isalpha` 和 `std::isdigit` 对负的 `char` 有未定义行为，可以只比较 ASCII 区间，或先把字符转成 `unsigned char`。
