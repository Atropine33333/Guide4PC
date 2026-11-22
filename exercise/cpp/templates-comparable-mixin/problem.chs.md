用 CRTP 写一个比较运算符混合类，让只定义了 `<` 和 `==` 的类型自动获得其余四个比较运算符。补全：

```cpp
template <typename Derived>
struct Comparable {
    // 由 Derived 提供 operator< 和 operator==
    // 在这里提供 !=、>、<=、>=
};

struct Entry : Comparable<Entry> {
    std::string name;
    int score = 0;
    int id = 0;

    Entry() = default;
    Entry(std::string n, int s, int i);

    bool operator<(const Entry& other) const;   // 自己实现
    bool operator==(const Entry& other) const;  // 自己实现
};
```

要求：

- `Comparable<Derived>` 用 `friend` 函数实现 `!=`、`>`、`<=`、`>=`，它们只调用 `Derived` 的 `==` 与 `<`，不要重复写比较逻辑。
- `Entry` 的排名规则：`score` 大者在前；`score` 相同时 `id` 小者在前；`score` 与 `id` 都相同则两个对象相等（`name` 也相同才算完全相等）。
- 排序、`std::max_element` 等标准算法只依赖 `<`，应当可以直接使用。

评测代码会检查 `Entry` 是否真的继承自 `Comparable<Entry>`，并验证六个运算符的一致性。**不要写 `main`。**

### 输入

无。

### 输出

无。

### 示例

```cpp
Entry a{"A", 90, 1};
Entry b{"B", 95, 2};
b > a;    // true，由混合类提供
a <= b;   // true
```

### 约束

- 只使用标准库。
- 六个运算符必须互相一致，禁止只为通过个别比较而写死返回值。
