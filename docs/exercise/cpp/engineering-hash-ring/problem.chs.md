（占卜师擦干净三百六十度的圆桌，开始洗牌。）

问卜者：这一局问什么？

占卜师：问一个名字落在哪一位值守的神明名下。桌上刻着 360 个刻度，从 0 数到 359。

问卜者：神明怎么摆？

占卜师：每位神明占 `virtual_nodes` 个位置，分身从 0 编号，名字写作 `name#i`。先把名字交给 FNV-1a：初值 `2166136261u`，每个字节先异或、再乘 `16777619u`，取 32 位回绕。再按牌面混合三次：`h ^= h >> 16`、乘 `0x7feb352du`；`h ^= h >> 15`、乘 `0x846ca68bu`；`h ^= h >> 16`。末了对 360 取余。

问卜者：那我问的名字呢？

占卜师：同样走一趟，落在哪个刻度，就顺时针找第一个不小于它的分身；越过末尾就绕回最小的那个。牌面落在谁手里，就是谁。

问卜者：添神、撤神呢？

占卜师：添一位已有的神，桌面不动；撤一位没有的神，桌面也不动。重复添神不生分身。`virtual_nodes` 小于 1 按 1 算。

问卜者：牌面会乱吗？

占卜师：添撤之后，只有本该换主人的名字才会换主人——这是这一行的手艺，评测就查这个。

**人话**：补完 `HashRing`。实现 FNV-1a 哈希与 32 位混合、`add_node`/`remove_node`，以及 `locate` 的顺时针查找与回绕；没有节点时返回 `""`。不要写 `main`。

```cpp
class HashRing {
public:
    explicit HashRing(int virtual_nodes = 50);

    void add_node(const std::string& name);
    void remove_node(const std::string& name);

    std::string locate(const std::string& key) const;  // 没有节点时返回 ""
    std::size_t size() const;                          // 不同节点个数
};
```

### 规则或约定

- 环上的位置范围是 `[0, 360)`。
- 每个节点有 `virtual_nodes` 个副本，第 `i` 个副本的字符串为 `name + "#" + std::to_string(i)`（从 0 开始）。
- 先算 32 位 FNV-1a 哈希（初值 `2166136261u`，每字节先异或再乘 `16777619u`），再做一次 32 位混合：`h ^= h >> 16; h *= 0x7feb352du; h ^= h >> 15; h *= 0x846ca68bu; h ^= h >> 16;`（无符号 32 位回绕），最后对 360 取余。
- `locate(key)` 取 `hash(key) % 360`，沿环找到第一个位置不小于它的副本；若越过末尾则回绕到最小的副本，返回该副本所属节点名。
- `add_node` 添加已存在的节点不产生任何变化；`remove_node` 移除不存在的节点是空操作。重复添加不能产生重复副本。
- `virtual_nodes < 1` 时按 1 处理。
- 评测代码会检查这些不变量，包括增删节点时“只有本应迁移的键才迁移”。不要写 `main`。

### 示例

```cpp
HashRing ring(50);
ring.add_node("a");
ring.locate("user:1");   // "a"
ring.add_node("b");
ring.locate("user:1");   // 仍可能是 "a"
```

### 约束

- 节点名只含小写字母，长度不超过 20。
- 测试中节点不超过 8 个，副本数不超过 200。
