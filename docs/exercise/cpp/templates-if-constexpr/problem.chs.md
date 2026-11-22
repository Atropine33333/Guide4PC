实现两个函数模板，用编译期分支处理不同类型：

```cpp
template <typename T>
std::string classify(const T& value);

template <typename T>
std::string render(const T& value);
```

`classify` 根据 `std::decay_t<T>` 返回下列字符串之一：

- `"integral"`：整数类型（包括 `bool` 和各种 `char`、`unsigned` 类型）；
- `"floating"`：`float`、`double`、`long double`；
- `"string"`：`std::string`；
- `"other"`：其余类型。

`render` 生成值的文本形式：

- 整数：十进制，例如 `42`、`-7`、`true` 渲染为 `"1"`；
- 浮点：固定两位小数，例如 `3.14159` 渲染为 `"3.14"`，`2.5` 渲染为 `"2.50"`；
- `std::string`：原样返回；
- 其他类型：返回 `"?"`。

要求使用 `if constexpr` 做编译期分派（不要写运行期 `if` 加类型特征的版本），并保证传入不可参与算术的类型时也能通过编译。

评测代码会直接包含你的实现，并用多种类型实例化这两个模板。**不要写 `main`。**

### 输入

无。

### 输出

无。

### 示例

```cpp
classify(42);                  // "integral"
classify(3.5);                 // "floating"
classify(std::string("hi"));   // "string"
render(3.14159);               // "3.14"
```

### 约束

- 只使用标准库。
- 不能为每种类型手写重载来绕过模板。
