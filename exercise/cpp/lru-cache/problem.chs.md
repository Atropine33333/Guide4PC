这一题不读 stdin，也不写 stdout。实现一个固定容量的 LRU（最近最少使用）缓存，测试在隐藏的 harness 里。

实现 `LruCache`：

- `explicit LruCache(int capacity)`：容量可以是 `0`。
- `bool get(int key, int& out)`：键存在时把值写入 `out`，把该键标记为最近使用，返回 `true`；不存在返回 `false`。
- `void put(int key, int value)`：插入或覆盖。键已存在时更新值并标记为最近使用；缓存已满且要插入新键时，先淘汰最久未使用的键。容量为 `0` 时什么也不做。
- `int size() const`：返回当前键的数量。
- `int capacity() const`：返回构造时的容量。

harness 会交替调用 `get` 和 `put`，检查淘汰的键是否恰好是最久未使用的那个，并用固定种子的随机序列与一份参考实现对比。
