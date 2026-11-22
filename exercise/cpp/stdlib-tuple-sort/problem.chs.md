这一题不读 stdin。实现记录的排序规则，隐藏的 harness 会调用它们。

```cpp
#include <string>
#include <vector>

struct Record {
    std::string name;
    int score;
    int id;
};

bool record_less(const Record& lhs, const Record& rhs);
void sort_records(std::vector<Record>& records);
```

排序规则（依次比较）：

1. `score` 大的在前；
2. `score` 相同时，`name` 按字典序小的在前（用 `std::string` 的 `<`，按字节比较）；
3. 仍然相同，`id` 小的在前。

要求：

- `record_less` 必须是一个严格弱序：`record_less(x, x)` 为 `false`，且对任意两个不同记录，`record_less(x, y)` 与 `record_less(y, x)` 恰好一个为真。
- `sort_records` 按上述规则原地排序，允许使用 `std::sort` 和 `record_less`。
- 空表和只有一个元素的表也必须能正常处理。
- 不要写 `main`。

### 提示

可以写成三层 `if`，也可以构造 `std::tuple(-score, name, id)` 后用 `operator<` 比较；注意分数取负时不要溢出（分数是普通 `int`，取负对 `INT_MIN` 不合法，需单独处理）。
