你只需要实现一个类，`push`、`pop` 和 `size` 会被直接调用。

实现 `CircularBuffer`，容量在构造时固定：

- `push(value)`：没满就写入并返回 `true`，满了返回 `false`；
- `pop(out)`：不为空就把最早写入的值取到 `out` 并返回 `true`，为空返回 `false`；
- `size()` / `capacity()`：返回当前元素数和总容量。

不要写 `main`。初始代码只是能编译的占位实现；把成员和方法补全即可。
