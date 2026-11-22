这一题不读 stdin。实现一个能存放“空”“整数”“字符串”三种状态的值类型，并正确管理拷贝与赋值。

```cpp
#include <string>

class AnyValue {
public:
    AnyValue();
    AnyValue(int value);
    AnyValue(const std::string& value);
    AnyValue(const AnyValue& other);
    AnyValue& operator=(const AnyValue& other);
    ~AnyValue();

    bool empty() const;
    bool is_int() const;
    bool is_string() const;
    int as_int(int fallback) const;
    std::string as_string(const std::string& fallback) const;
    std::string describe() const;
};
```

约定：

- 默认构造得到空状态；`describe()` 返回 `"empty"`。
- 整数状态的 `describe()` 返回 `"int:"` 加上十进制值，例如 `"int:-7"`。
- 字符串状态的 `describe()` 返回 `"string:"` 加上内容；空字符串也是合法的字符串状态，`describe()` 为 `"string:"`。
- `as_int` 只在整数状态下返回内部值（包括 `0`），其他状态返回 `fallback`。
- `as_string` 只在字符串状态下返回内部值（包括空串），其他状态返回 `fallback`。
- 拷贝必须是深拷贝；修改副本不能影响原对象。赋值要能处理自赋值。
- 不要写 `main`。

### 提示

可以用一个抽象基类保存数据，用 `std::unique_ptr` 持有它，并在拷贝构造/赋值时调用虚函数 `clone()`；也可以选择其他等价的类型擦除表示。由于析构、拷贝、赋值都需要自定义，这是“rule of five”的练习。
