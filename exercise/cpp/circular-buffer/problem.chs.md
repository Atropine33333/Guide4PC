这一题不读 stdin，也不写 stdout。你只需要实现一个类，测试逻辑放在隐藏的 harness 里：构建时 harness 会拼在你的代码后面，直接调用 `push`、`pop` 和 `size`。

实现 `CircularBuffer`，容量在构造时固定：

- `push(value)`：没满就写入并返回 `true`，满了返回 `false`；
- `pop(out)`：不为空就把最早写入的值取到 `out` 并返回 `true`，为空返回 `false`；
- `size()` / `capacity()`：返回当前元素数和总容量。

不要写 `main`：harness 会提供程序入口。初始代码只是能编译的占位实现，所有检查都会失败；把成员和方法补全即可。
