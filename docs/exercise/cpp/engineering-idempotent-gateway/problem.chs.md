这一题不读 stdin，也不写 stdout。实现一个幂等的提交网关。

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

规则：

- 新编号第一次 `submit` 返回 `accepted`，记录金额，进入待完成状态。
- 已存在且金额相同，返回 `duplicate`，不改变任何状态。
- 已存在但金额不同，返回 `conflict`，不改变任何状态。
- `complete` 只能完成一个存在且尚未完成的编号，记录结果并返回 `true`；未知编号或已经完成返回 `false`。
- `result_of` 只在编号已经完成时返回结果，否则返回 `std::nullopt`。
- 已完成的编号再次提交相同金额仍返回 `duplicate`，再次提交不同金额仍返回 `conflict`；完成状态和结果都不能被清除。
- `pending()` 是存在但未完成的编号数；`completed()` 是已完成的编号数。
- 编号和金额都可以是负数。
- 不要写 `main`，不要修改公开接口。
