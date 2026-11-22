《可容值对象法典》

第一条（宗旨）为规范“空”“整数”“字符串”三态值对象的构造、拷贝与销毁，制定本法典。

第二条（三态）默认构造者为空态，`describe()` 报 `"empty"`。整数态报 `"int:"` 接十进制值，如 `"int:-7"`。字符串态报 `"string:"` 接内容；空串亦为合法字符串态，报 `"string:"`。

第三条（取值）`as_int` 只在整数态交出所存之值（含 `0`），余者交出 `fallback`；`as_string` 只在字符串态交出所存之值（含空串），余者交出 `fallback`。

第四条（拷贝）拷贝须为深拷贝，改动副本不得累及原件；赋值须能处置自赋值。

第五条（施行）析构、拷贝、赋值三者皆须自定，故本法典又称“五法之则”。实现自择，或托虚基类与 `std::unique_ptr`，或另立他法，唯语义不得有违。

第六条（附则）不得写 `main`。

**人话**：实现三种状态的值类型 `AnyValue`：默认是空态，另有整数态与字符串态；`describe()` 按 `"empty"`、`"int:-7"`、`"string:abc"` 的格式报告，`as_int` 与 `as_string` 只在对应状态交出内部值（含 `0` 与空串），否则交出 `fallback`。拷贝必须是深拷贝，赋值要能处理自赋值，析构、拷贝、赋值三者都要自定义——这是 rule of five 的练习。

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

### 提示

可以用一个抽象基类保存数据，用 `std::unique_ptr` 持有它，并在拷贝构造/赋值时调用虚函数 `clone()`；也可以选择其他等价的类型擦除表示。由于析构、拷贝、赋值都需要自定义，这是“rule of five”的练习。
