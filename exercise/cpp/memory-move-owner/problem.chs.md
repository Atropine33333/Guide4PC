这一题不读 stdin，也不写 stdout。实现一个只能移动、不能拷贝的动态整型数组，测试在隐藏的 harness 里。

实现：

```cpp
class UniqueIntArray {
public:
    explicit UniqueIntArray(int n);  // n >= 0，元素全部初始化为 0
    ~UniqueIntArray();

    UniqueIntArray(const UniqueIntArray&) = delete;
    UniqueIntArray& operator=(const UniqueIntArray&) = delete;

    UniqueIntArray(UniqueIntArray&& other) noexcept;
    UniqueIntArray& operator=(UniqueIntArray&& other) noexcept;

    int size() const;
    int* data();
    const int* data() const;
    void set(int index, int value);  // 0 <= index < size()
    int get(int index) const;        // 0 <= index < size()
};
```

移动之后：

- 目标对象接管原来的缓冲区、长度和全部元素；
- 源对象的 `size()` 变为 0，`data()` 变为 `nullptr`；
- 不得出现双重释放；零长度数组的 `data()` 也是 `nullptr`。

不要写 `main`。
