耶和华晓谕摩西说：

「你要按以色列支派的数目，为每一支派取一份。份数是 `N`，在起初就定好了，出埃及的路上不可增减。」

「每一份起初是空的。整数归零，浮点归零，好叫他们知道：份量先于内容，仓房先于粮食。」

「凡取份的，要按着次序。`operator[]` 只认已经定好的位置。若有人问起界外的位置，`try_get` 不可给他，也不可动他手中的器皿，只回一句 `false`。」

「你要用实打实的仓房存放，不可用会自己伸缩的皮袋。`T items_[N]` 便是这样的仓房；`{}` 一落，众份皆空。」

「空的份也要能填。`fill` 一发，举仓皆同。」

摩西便照耶和华所吩咐的行了。

**人话**：实现类模板 `FixedArray<T, N>`，内部用真正的定长存储，默认构造把全部元素值初始化，`try_get` 检查 `0 <= index < N`。

```cpp
template <typename T, int N>
class FixedArray {
public:
    FixedArray();       // 所有 N 个元素值初始化为 T{}

    int size() const;   // 返回 N

    T& operator[](int index);
    const T& operator[](int index) const;

    void fill(const T& value);  // 把所有元素设为 value

    // 越界时返回 false，且不修改 out；成功时写入 out 并返回 true。
    bool try_get(int index, T& out) const;

    T* data();
    const T* data() const;
};
```

要求：

- 内部使用真正的定长存储（例如 `T items[N]`），不要用 `std::vector` 动态分配；
- `FixedArray<T, N>` 的默认构造必须把 `int` 元素初始化为 0、`double` 元素初始化为 0.0；
- `operator[]` 假定下标合法；`try_get` 必须检查 `0 <= index < N`；
- 不要写 `main`。

提示：`T items_[N]{};` 的 `{}` 会做值初始化。
