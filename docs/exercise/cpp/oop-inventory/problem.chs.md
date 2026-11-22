这一题不读 stdin，也不写 stdout。实现一个简单的库存账本。

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

规则：

- `add` 在已有数量上累加；`quantity <= 0` 时忽略本次调用。
- `remove` 在数量足够时扣除并返回 `true`；数量恰好扣到 0 时删除该名称。
- 名称不存在、数量不足或 `quantity <= 0` 时返回 `false`，库存不能发生任何变化。
- `quantity` 对不存在的名称返回 0。
- `distinct()` 返回当前数量为正的不同名称数。
- `total()` 返回所有名称的数量之和，返回值使用 64 位整数。
- 名称区分大小写，可以包含空格，也可以是空字符串。
- 不要写 `main`，不要修改公开接口。
