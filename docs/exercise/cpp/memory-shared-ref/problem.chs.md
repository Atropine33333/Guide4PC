这一题不读 stdin，也不写 stdout。实现一个带引用计数的共享整数，测试在隐藏的 harness 里。

实现：

```cpp
class SharedInt {
public:
    explicit SharedInt(int value = 0);
    SharedInt(const SharedInt& other);
    SharedInt& operator=(const SharedInt& other);
    ~SharedInt();

    int value() const;
    void set_value(int value);
    int use_count() const;  // 当前共享同一块数据的对象数
};
```

要求：

- 每个对象通过同一个控制块共享一个 `int` 值和一个引用计数；
- 拷贝构造、拷贝赋值都让计数加一，并共享值；
- 赋值要先释放左操作数原来的引用，再共享右操作数的数据；自赋值必须安全；
- 析构时计数减一，减到 0 才释放控制块；
- 通过任意一个副本调用 `set_value`，其他副本都能看到新值；
- 不要写 `main`。

提示：可以用一个私有的嵌套 `struct Control { int value; int count; };`，对象里只存 `Control*`。
