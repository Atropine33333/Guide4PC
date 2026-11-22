这一题不读 stdin。根据一份简单的事务日志，重放出键值存储的最终状态。

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

约定：

- `put` 在**没有活跃事务**时立即写入存储；在活跃事务中先记入事务缓冲区（同一键后写覆盖先写）。
- `begin` 开启事务；如果事务已经活跃，忽略这次 `begin`（只支持扁平事务）。
- `commit` 把缓冲区中的写入按顺序应用到存储，然后清空缓冲区、结束事务；没有活跃事务时忽略。
- `rollback` 丢弃缓冲区并结束事务；没有活跃事务时忽略。
- 未知的 `op` 忽略。
- 日志结束时若事务仍未结束，缓冲区里的写入全部丢弃（相当于隐式回滚）。
- 返回最终存储中的键值对，按键的字典序（`std::string` 的 `<`）升序排列。
- 不要写 `main`。

### 示例

日志：`put a 1`、`begin`、`put a 5`、`put b 3`、`commit`。最终结果是 `[("a", 5), ("b", 3)]`。

如果最后一条不是 `commit` 而是 `rollback`，结果是 `[("a", 1)]`。

### 提示

用两个 `std::map<std::string, int>`：一个保存已提交的存储，一个保存事务缓冲；再加一个 `bool active`。返回时把已提交的 map 转成 vector 即可，map 的迭代顺序已经是字典序。
