这一题不读 stdin，也不写 stdout。实现一个小型命令路由表，测试在隐藏的 harness 里。

```cpp
#include <functional>
#include <string>

class CommandRouter {
public:
    using Handler = std::function<std::string(const std::string&)>;

    void add(const std::string& name, Handler handler);
    bool has(const std::string& name) const;
    std::string dispatch(const std::string& name, const std::string& argument) const;
    std::size_t size() const;
};
```

规则：

- `add` 注册一个命令；重复注册同名命令时，新处理函数替换旧的，`size()` 不增加。
- `has` 判断命令是否注册过。
- `dispatch(name, argument)` 调用对应处理函数并返回它的结果；`dispatch` 是 `const` 成员函数。
- 未知命令返回字符串 `"unknown"`，不调用任何处理函数。
- 处理函数可以捕获外部状态（例如计数器），`dispatch` 为 `const` 不妨碍处理函数修改它捕获的变量。
- 不要写 `main`。

### 示例

| 调用序列 | 结果 |
| --- | --- |
| `dispatch("nope", "")` | `unknown` |
| `add("echo", ...)` 后 `dispatch("echo", "hi")` | `hi` |
| 同名 `add` 两次 | `size()` 仍为 1，调用新的处理函数 |

### 提示

用 `std::map<std::string, Handler>` 保存命令，`add` 直接 `handlers_[name] = std::move(handler);`。`dispatch` 中先 `find`，找不到就返回 `"unknown"`，找到则调用 `it->second(argument)`。注意成员函数声明为 `const` 后，`handlers_` 也被视为 `const`，但 `std::function::operator()` 本身是 `const`，可以调用。
