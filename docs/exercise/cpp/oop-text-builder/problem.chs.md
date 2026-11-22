实现一个逐步构建文本的类。

```cpp
class TextBuilder {
public:
    TextBuilder();
    explicit TextBuilder(const std::string& initial);

    TextBuilder& append(const std::string& text);
    TextBuilder& append(char c);
    TextBuilder& operator+=(const std::string& text);
    TextBuilder operator+(const std::string& text) const;

    const std::string& str() const;
    int size() const;
    bool empty() const;
    void clear();

    char& operator[](int index);
    const char& operator[](int index) const;

    bool operator==(const TextBuilder& other) const;
    bool operator<(const TextBuilder& other) const;
};

std::ostream& operator<<(std::ostream& os, const TextBuilder& builder);
```

规则：

- 默认构造为空串；`TextBuilder(initial)` 从给定文本开始。
- `append` 和 `+=` 把文本追加到末尾，返回 `*this` 的引用，因此可以链式调用。
- `operator+` 返回一个**新对象**：内容是原对象加上参数；原对象不变。它是 `const` 成员函数。
- `str` 返回内部字符串的 const 引用；`size` 返回字符数；`empty` 判断是否为空；`clear` 清空。
- `operator[]` 支持读写，下标保证合法；const 版本返回 `const char&`。
- `==` 比较文本内容；`<` 按字典序比较内容。
- `operator<<` 把内部文本写到流里，并返回该流。
- 不要写 `main`。

### 提示

内部用一个 `std::string` 保存文本。`operator+` 可以先拷贝 `*this`，再对副本调用 `append`，最后返回副本。`operator<<` 是自由函数，直接写 `os << builder.str()`。
