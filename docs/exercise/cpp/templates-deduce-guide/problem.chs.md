实现一个序列类模板，并为它补上推导指引，让调用者不必写模板实参：

```cpp
template <typename T>
class Seq {
public:
    Seq() = default;
    Seq(std::initializer_list<T> items);
    template <typename It>
    Seq(It first, It last);
    template <typename U, std::size_t N>
    Seq(const U (&items)[N]);

    std::size_t size() const;
    T sum() const;                      // 空序列返回 T{}
    bool contains(const T& value) const;
    const T& at(std::size_t index) const;

private:
    std::vector<T> items_;
};

template <typename It>
Seq(It, It) -> Seq<typename std::iterator_traits<It>::value_type>;

template <typename T, std::size_t N>
Seq(const T (&)[N]) -> Seq<T>;
```

要求：

- `Seq s{1, 2, 3}` 推导为 `Seq<int>`；
- `Seq q(v.begin(), v.end())`（`v` 是 `std::vector<long long>`）推导为 `Seq<long long>`；
- `Seq a(raw)`（`raw` 是 `long long raw[4]`）推导为 `Seq<long long>`；
- 也允许显式写 `Seq<int> empty;`；
- `sum()` 对空序列返回 `T{}`。

评测代码会检查推导出的类型（通过 `std::is_same_v`）以及各成员函数的结果。**不要写 `main`。**

### 输入

无。

### 输出

无。

### 示例

```cpp
Seq s{1, 2, 3};               // Seq<int>
std::vector<long long> v{5};  // v.begin() 的 value_type 是 long long
Seq q(v.begin(), v.end());    // Seq<long long>
```

### 约束

- 只使用标准库。
- 不能通过重载 `Seq` 的非模板别名绕过推导。
