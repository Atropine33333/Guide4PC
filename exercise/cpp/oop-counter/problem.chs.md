这一题不读 stdin，也不写 stdout。实现一个计数器类，测试在隐藏的 harness 里。

```cpp
class Counter {
public:
    Counter();
    explicit Counter(int start);

    void increment();
    void decrement();
    int value() const;
    void reset();

    Counter& operator++();
    Counter operator++(int);
    Counter& operator+=(int amount);
};
```

规则：

- 默认构造的计数器为 0；`Counter(start)` 在 `start < 0` 时也取 0。
- `increment` 加 1；`decrement` 减 1，但不会减到 0 以下。
- `value` 返回当前值；`reset` 把值置 0。
- 前置 `++` 先加 1，再返回自身的引用。
- 后置 `++` 返回加 1 之前的**值**（返回类型是 `Counter`，不是引用），然后把自身加 1。
- `+=` 允许负数；结果小于 0 时取 0，返回自身的引用。
- 拷贝一个计数器会得到独立的值。
- 不要写 `main`。

### 提示

后置 `++` 用一个临时对象保存旧值并返回它。注意区分返回引用和返回值。
