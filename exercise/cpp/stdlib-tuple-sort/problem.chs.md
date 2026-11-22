《记录排序法典》

第一条（目的）为明确记录之间的先后次序，避免比较函数违反严格弱序，制定本法典。

第二条（分数）`score` 大者在前。

第三条（姓名）分数相同时，`name` 按字典序小者在前；比较采用 `std::string` 的 `<`，逐字节进行。

第四条（编号）分数与姓名都相同时，`id` 小者在前。

第五条（严格弱序）`record_less` 必须满足：`record_less(x, x)` 为 `false`；对任意两个不同记录，`record_less(x, y)` 与 `record_less(y, x)` 恰有一个为真。

第六条（原地排序）`sort_records` 按上述规则原地排序，可以使用 `std::sort` 与 `record_less`。空表与只有一个元素的表也须正常处理。

第七条（附则）实现下列接口，不得编写 `main`。分数是普通 `int`，若以取负构造 `std::tuple`，须当心 `INT_MIN`，单独处理。

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

**人话**：实现 `record_less` 与 `sort_records`。排序规则依次为：`score` 大的在前；分数相同时 `name` 字典序小的在前（按字节比较）；仍然相同则 `id` 小的在前。`record_less` 必须是严格弱序：`record_less(x, x)` 为 `false`，任意两个不同记录中恰有一个方向为真。`sort_records` 原地排序，可以使用 `std::sort`；空表和单元素表也要正常处理。不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `record_less({"a", 10, 1}, {"b", 10, 2})` | `true` |
| `record_less({"a", 10, 2}, {"a", 10, 2})` | `false` |

### 提示

可以写成三层 `if`，也可以构造 `std::tuple(-score, name, id)` 后用 `operator<` 比较；注意分数取负时不要溢出（分数是普通 `int`，取负对 `INT_MIN` 不合法，需单独处理）。
