这一题不读 stdin，也不写 stdout。实现一个能统计“当前存活对象数”的类，测试在隐藏的 harness 里。

实现 `Tracked`：

- `Tracked()`：构造，值为 `0`。
- `explicit Tracked(int value)`：构造，值为 `value`。
- `Tracked(const Tracked& other)`：拷贝构造，复制值；这是一个**新对象**。
- `Tracked& operator=(const Tracked& other)`：赋值，复制值；对象数量不变，自赋值安全。
- `~Tracked()`：析构。
- `int value() const` 与 `void setValue(int value)`：读写值。
- `static int alive()`：返回当前存活的对象个数。

初始时存活数为 `0`。构造和拷贝构造都让存活数加一，析构让它减一，赋值不改变存活数。harness 会在不同作用域里创建对象、拷贝对象、放入数组，并在作用域结束时检查计数回到零。
