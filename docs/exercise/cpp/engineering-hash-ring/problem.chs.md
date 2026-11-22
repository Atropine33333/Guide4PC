实现一致性哈希环：

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

规则：

- 环上的位置范围是 `[0, 360)`。
- 每个节点有 `virtual_nodes` 个副本，第 `i` 个副本的字符串为 `name + "#" + std::to_string(i)`（从 0 开始）。
- 先算 32 位 FNV-1a 哈希（初值 `2166136261u`，每字节先异或再乘 `16777619u`），再做一次 32 位混合：
  `h ^= h >> 16; h *= 0x7feb352du; h ^= h >> 15; h *= 0x846ca68bu; h ^= h >> 16;`
  （无符号 32 位回绕），最后对 360 取余。
- `locate(key)` 取 `hash(key) % 360`，沿环找到第一个位置不小于它的副本；若越过末尾则回绕到最小的副本。返回该副本所属节点名。
- `add_node` 添加已存在的节点不产生任何变化；`remove_node` 移除不存在的节点是空操作。重复添加不能产生重复副本。
- `virtual_nodes < 1` 时按 1 处理。

评测代码会检查这些不变量，包括增删节点时“只有本应迁移的键才迁移”。**不要写 `main`。**

### 输入

无。

### 输出

无。

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
