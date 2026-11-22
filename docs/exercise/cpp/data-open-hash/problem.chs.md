这一题不读 stdin。用开放寻址法（线性探测）实现一个整数哈希表。

```cpp
#include <vector>

class OpenHashMap {
public:
    void put(int key, int value);
    bool get(int key, int& out) const;
    bool erase(int key);
    int size() const;
};
```

约定：

- 键和值都是 `int`，键可以为负数。
- 同一个键重复 `put` 时更新它的值，不增加元素个数。
- `get` 找到时把值写入 `out` 并返回 `true`；找不到返回 `false`，且**不修改** `out`。
- `erase` 删除键并返回 `true`；键不存在时返回 `false`。
- `size` 返回当前键值对数量。
- 必须使用开放寻址（线性探测）；删除后留下的空位不能让后续探测提前终止。
- 不要写 `main`。

### 提示

每个槽位可以记录三种状态：空、占用、已删除（墓碑）。探测时遇到墓碑继续往后找，遇到真正的空槽才停止。元素变多时按需扩容并重新插入所有仍占用的槽位。
