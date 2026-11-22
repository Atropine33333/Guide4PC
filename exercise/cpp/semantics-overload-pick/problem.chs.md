这一题不读 stdin，也不写 stdout。实现三个同名重载，测试在隐藏的 harness 里。

实现：

- `const char* pick(int value)`：返回 `"int"`。
- `const char* pick(long value)`：返回 `"long"`。
- `const char* pick(double value)`：返回 `"double"`。

harness 会传入不同类型的实参，并检查 C++ 的重载决议最终选中了哪一个。不要写 `main`。
