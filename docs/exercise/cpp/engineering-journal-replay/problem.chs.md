耶和华晓谕摩西说：

你要照这日志，把百姓所经的每一站重走一遍。日志上写 `put`，就在会幕外的石板上刻下名字与数目；若那时正在安营（事务未了），先记在帐内，同一名字后刻的盖过先刻的。

写 `begin`，就支起帐幕；帐幕已经支着，再写 `begin`，当作没看见，只容一顶帐幕。

写 `commit`，把帐内的条目按次序搬到石板上，然后收帐；写 `rollback`，把帐内的条目全部抹去，然后收帐。没有帐幕的时候写这两个字，都当作没看见。

不认得的字，略过。

走到最后一站，帐幕还支着，帐内的条目一概不算。

摩西问：石板上的名字怎么排？耶和华说：按名字的字典序，从小到大。

**人话**：实现 `replay`，返回日志重放后的键值对，按键的 `std::string` 字典序升序排列。用两个 `std::map`（已提交的存储、事务缓冲）加一个 `bool active`；`commit` 时把缓冲按顺序并入，逐条按顺序应用。不要写 `main`。

```cpp
#include <string>
#include <utility>
#include <vector>

struct JournalEntry {
    std::string op;   // "put" / "begin" / "commit" / "rollback"
    std::string key;
    int value;
};

std::vector<std::pair<std::string, int>> replay(
    const std::vector<JournalEntry>& entries);
```

### 规则或约定

- `put` 在没有活跃事务时立即写入存储；在活跃事务中先记入事务缓冲区（同一键后写覆盖先写）。
- `begin` 开启事务；如果事务已经活跃，忽略这次 `begin`（只支持扁平事务）。
- `commit` 把缓冲区中的写入按顺序应用到存储，然后清空缓冲区、结束事务；没有活跃事务时忽略。
- `rollback` 丢弃缓冲区并结束事务；没有活跃事务时忽略。
- 未知的 `op` 忽略。
- 日志结束时若事务仍未结束，缓冲区里的写入全部丢弃（相当于隐式回滚）。
- 返回最终存储中的键值对，按键的字典序（`std::string` 的 `<`）升序排列。
- 不要写 `main`。

### 示例

日志：`put a 1`、`begin`、`put a 5`、`put b 3`、`commit`。最终结果是 `[("a", 5), ("b", 3)]`。

如果最后一条换成 `rollback`，结果会是 `[("a", 1)]`。

### 提示

用两个 `std::map<std::string, int>`：一个保存已提交的存储，一个保存事务缓冲；再加一个 `bool active`。返回时把已提交的 map 转成 vector 即可，map 的迭代顺序已经是字典序。
