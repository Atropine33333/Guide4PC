为同一个函数名写三个重载，让调用者能区分左值、`const` 左值和右值。

```cpp
#include <string>

std::string describe(int& value);         // 返回 "lvalue"
std::string describe(const int& value);   // 返回 "const-lvalue"
std::string describe(int&& value);        // 返回 "rvalue"
```

规则：

- 三个重载的返回字符串必须分别是 `lvalue`、`const-lvalue`、`rvalue`。
- 不要修改参数（即使是 `int&` 版本）。
- 不要写 `main`。

### 提示

重载决议会优先选择更匹配的引用类型：非 const 左值选 `int&`，const 左值只能选 `const int&`，临时对象和 `std::move(x)` 选 `int&&`。只写一个按值接收的版本，所有调用都会落到它上面。
