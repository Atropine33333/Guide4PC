这一题不读 stdin，也不写 stdout。实现三个处理 `std::variant` 的函数。

```cpp
#include <string>
#include <variant>

using Value = std::variant<int, double, std::string>;

std::string kind_of(const Value& value);

double as_double(const Value& value);

std::string describe(const Value& value);
```

规则：

- `kind_of` 返回 `"int"`、`"double"` 或 `"string"`，分别对应当前活动的类型。
- `as_double`：
  - `int` 转成对应的 `double`；
  - `double` 原样返回；
  - `std::string` 返回 `0.0`。
- `describe` 返回下面的精确格式：
  - `int`：`int(42)`、`int(-7)`；
  - `double`：保留两位小数，使用固定格式，例如 `double(3.50)`、`double(-0.25)`、`double(100.00)`；
  - `std::string`：`string(hello)`，字符串为空时是 `string()`。
- 请使用 `std::visit` 或 `std::holds_alternative`，不要依赖未定义行为。
- 不要写 `main`，不要修改类型别名和函数签名。
