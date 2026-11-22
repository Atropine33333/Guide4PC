甲：我这儿有个类型，只会两样本事：`<` 和 `==`。

乙：那可不够使。排序看 `<`，可您还想问“大于”“不大于”“不等于”。

甲：所以我给它请了位师傅，叫 `Comparable<Derived>`。CRTP，徒弟把自己当模板参数，送进师傅门里。

乙：师傅教它什么？

甲：教四样：`!=`、`>`、`<=`、`>=`。全拿徒弟那两样本事推出来，师傅不另写一套比较逻辑。

乙：那 `Entry` 怎么论先后？

甲：分数大的在前；分数一样，编号小的在前；分数、编号、名字全对上，才叫相等。

乙：六样本事得说一样的话。别嘴上说大于，手上又说不大于。

甲：那是自然。六个运算符互相一致，排序和 `std::max_element` 才敢放心用。

乙：那我把两个类型补齐？

甲：补齐吧。师傅那段用 `friend` 函数写，徒弟那段自己写 `<` 与 `==`。

**人话**：补全下面两个类型：`Comparable<Derived>` 用 `friend` 函数提供 `!=`、`>`、`<=`、`>=`，它们只调用 `Derived` 的 `==` 与 `<`；`Entry` 自己实现 `operator<` 与 `operator==`。**不要写 `main`。**

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

`Entry` 必须真正继承自 `Comparable<Entry>`，六个运算符必须保持一致。**不要写 `main`。**

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
