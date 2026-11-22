这一题不读 stdin，也不写 stdout。实现一个可以撤销最近一次写入的键值存储，测试在隐藏的 harness 里。

实现 `VersionedStore`：

- `VersionedStore()`：构造空存储。
- `void put(int key, int value)`：插入或覆盖一个键，并记录这次写入，使它成为最近一次可撤销的操作。
- `bool get(int key, int& out) const`：键存在时把值写入 `out` 并返回 `true`，否则返回 `false`。
- `bool undo()`：撤销最近一次 `put`。如果那次是覆盖，恢复原来的值；如果那次是插入，删除这个键；返回 `true`。没有可撤销的操作时返回 `false`。
- `int size() const`：返回当前键的数量。
- `void clear()`：清空所有键，**同时丢弃全部撤销历史**。之后 `undo()` 返回 `false`。

harness 会按时间顺序检查每一次撤销的结果，并运行一段固定种子的随机操作序列。
