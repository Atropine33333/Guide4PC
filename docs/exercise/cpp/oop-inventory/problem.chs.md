天国好像一个家主，把仓房的钥匙交给仆人，吩咐说：“凡有货物进来的，照数记在账上；凡有取走的，先看账上够不够。够的，扣除；扣到一粒不剩，就把这名目从册上抹去。不够的，或数目不正的，一概不许动账，只回一个‘不成’。”

仆人问：“同一个名目，进来两回呢？”

家主说：“加在一处，不必另立一行。账上的名目，大小写各算各的，带空格的、空着名的，也各算各的。月底盘账，只数个名目、加个总数；总数要大些，用 64 位的数才装得下。”

**人话**：实现库存账本类 `Inventory`，底层是字符串到整数的映射：`add` 把数量累加到已有数目上（`quantity <= 0` 时忽略）；`remove` 在存量足够时扣除并返回 `true`，正好扣到 0 就删除该名目，否则返回 `false` 且库存不变；`quantity(name)` 查数量（不存在的名目返回 0）；`distinct()` 数当前数量为正的名目个数；`total()` 用 64 位整数汇总全部数量。

```cpp
#include <cstddef>
#include <string>

class Inventory {
public:
    Inventory();

    void add(const std::string& name, int quantity);
    bool remove(const std::string& name, int quantity);
    int quantity(const std::string& name) const;
    std::size_t distinct() const;
    long long total() const;
};
```

### 规则

- `add` 在已有数量上累加；`quantity <= 0` 时忽略本次调用。
- `remove` 在数量足够时扣除并返回 `true`；数量恰好扣到 0 时删除该名称。
- 名称不存在、数量不足或 `quantity <= 0` 时返回 `false`，库存不能发生任何变化。
- `quantity` 对不存在的名称返回 0。
- `distinct()` 返回当前数量为正的不同名称数。
- `total()` 返回所有名称的数量之和，返回值使用 64 位整数。
- 名称区分大小写，可以包含空格，也可以是空字符串。
- 不要写 `main`，不要修改公开接口。
