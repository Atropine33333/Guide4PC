《幂等提交条例》

第一条 为杜绝重复入账与重复受理，制定本条例。

第二条 凡编号未曾出现者，首次提交记为受理（`accepted`），留存金额，置于待完成。

第三条 编号已存在且金额相同者，答以重复（`duplicate`），一切照旧。金额不同者，答以冲突（`conflict`），一切照旧。

第四条 完成只对存在且尚未完成的编号开放，记下结果，返回 `true`；编号不存在或已经完成，返回 `false`。已经完成者，状态与结果永不清除。

第五条 查询结果只对已经完成者给出结果；其余答以 `std::nullopt`。

第六条 待完成数只数存在而未完成者，已完成数只数已完成者。编号与金额都可以为负。

第七条 不要写 `main`，公开接口不得增删改。

**人话**：补完 `Gateway`，按上面的状态与返回码实现 `submit`、`complete`、`is_completed`、`result_of`、`pending`、`completed`。重复提交不影响已有记录，已完成的记录不退回待完成。

```cpp
#include <cstddef>
#include <optional>

enum class SubmitStatus { accepted, duplicate, conflict };

class Gateway {
public:
    Gateway();

    SubmitStatus submit(int id, int amount);
    bool complete(int id, int result);
    bool is_completed(int id) const;
    std::optional<int> result_of(int id) const;
    std::size_t pending() const;
    std::size_t completed() const;
};
```

### 规则或约定

- 新编号第一次 `submit` 返回 `accepted`，记录金额，进入待完成状态。
- 已存在且金额相同，返回 `duplicate`，不改变任何状态。
- 已存在但金额不同，返回 `conflict`，不改变任何状态。
- `complete` 只能完成一个存在且尚未完成的编号，记录结果并返回 `true`；未知编号或已经完成返回 `false`。
- `result_of` 只在编号已经完成时返回结果，否则返回 `std::nullopt`。
- 已完成的编号再次提交相同金额仍返回 `duplicate`，再次提交不同金额仍返回 `conflict`；完成状态和结果都不能被清除。
- `pending()` 是存在但未完成的编号数；`completed()` 是已完成的编号数。
- 不要写 `main`，不要修改公开接口。
