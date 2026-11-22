这一题不读 stdin，也不写 stdout。实现一个整数栈，并让它可以随时报告当前最小值；测试在隐藏的 harness 里。

实现 `MinStack`，提供以下成员：

- `void push(int value)`：把 `value` 压栈。
- `bool pop()`：弹出栈顶并返回 `true`；栈空时返回 `false`，不改变栈。
- `bool top(int& out) const`：若栈非空，把栈顶写入 `out` 并返回 `true`；否则返回 `false`。
- `bool min(int& out) const`：若栈非空，把当前最小值写入 `out` 并返回 `true`；否则返回 `false`。
- `int size() const`：返回元素个数。

所有操作都应当是 O(1)。提示：只保存一个最小值是不够的，最小元素出栈后要能恢复之前的最小值，可以再维护一个辅助栈。
