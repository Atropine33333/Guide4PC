这一题不读 stdin，也不写 stdout。实现一个固定容量的整数对象池，测试在隐藏的 harness 里。

```cpp
class IntPool {
public:
    explicit IntPool(int capacity);
    ~IntPool();

    IntPool(const IntPool&) = delete;
    IntPool& operator=(const IntPool&) = delete;

    int* acquire(int value);
    bool release(int* slot);
    int live() const;
    int capacity() const;

private:
    int* slots_;
    bool* used_;
    int capacity_;
    int live_;
};
```

规则：

- `capacity >= 0`；容量为 0 时 `acquire` 永远返回 `nullptr`，`release(nullptr)` 返回 `false`。
- 构造时分配 `capacity` 个槽位，初始都未使用。
- `acquire(value)` 找到第一个空闲槽位，标记为已使用，写入 `value`，返回该槽位的地址；池满时返回 `nullptr`。空闲槽位应从下标 0 开始找。
- `release(slot)` 只有当 `slot` 正好指向池内某个**当前已使用**的槽位时才返回 `true` 并释放它；其他情况（外部指针、`nullptr`、已经释放过）返回 `false`，且不改变状态。
- `live()` 返回当前已分配的槽位数；`capacity()` 返回总容量。
- 析构函数释放内部数组，容量为 0 时也要安全。
- 拷贝构造和拷贝赋值已删除，不要实现它们。
- 不要写 `main`。

### 提示

`slots_` 保存值，`used_` 记录每个槽位是否已借出。`release` 可以遍历槽位，用地址相等判断指针是否属于池，而不是做指针大小比较。
