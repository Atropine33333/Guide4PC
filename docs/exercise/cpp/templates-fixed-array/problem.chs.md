这一题不读 stdin，也不写 stdout。实现一个定长数组类模板，测试在隐藏的 harness 里。

实现：

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
