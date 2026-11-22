这一题不读 stdin，也不写 stdout。实现一个自己管理内存、可以增长的整数数组，测试在隐藏的 harness 里。

```cpp
class IntVector {
public:
    IntVector();
    ~IntVector();
    IntVector(const IntVector& other);
    IntVector& operator=(const IntVector& other);
    IntVector(IntVector&& other) noexcept;
    IntVector& operator=(IntVector&& other) noexcept;

    void push_back(int value);
    int size() const;
    int capacity() const;
    void clear();
    int& operator[](int index);
    const int& operator[](int index) const;
    int* data();
    const int* data() const;

private:
    int* data_;
    int size_;
    int capacity_;
};
```

规则（rule of five）：

- 默认构造：`size() == 0`、`capacity() == 0`、`data() == nullptr`。
- `push_back` 在尾部追加；容量不够时重新分配（容量至少为 1，且每次增长后 `capacity() >= size()`）。旧元素必须保持顺序。
- 析构函数用 `delete[]` 释放内存，空数组也要安全。
- 拷贝构造和拷贝赋值做深拷贝：两个数组之后互不影响。
- 移动构造和移动赋值是 `noexcept`：偷走指针，并让源对象变成空数组（`size() == 0`、`data() == nullptr`）。
- 自赋值安全，且保持原值。
- `clear()` 把 `size()` 置 0，不要求释放容量；之后再 `push_back` 必须能正常工作。
- `operator[]` 的下标保证合法；`data()` 返回连续存储的首地址。
- 不要写 `main`。

### 提示

扩容时先分配新数组，再逐个拷贝旧元素，最后 `delete[]` 旧数组。移动后把源对象的三个成员都置成空状态。
