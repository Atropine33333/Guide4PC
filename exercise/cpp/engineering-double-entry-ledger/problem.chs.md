这一题不读 stdin，也不写 stdout。实现一个支持原子转账的账本。

```cpp
#include <cstddef>
#include <optional>

class Ledger {
public:
    Ledger();

    bool open(int account);
    bool deposit(int account, long long amount);
    bool withdraw(int account, long long amount);
    bool transfer(int from, int to, long long amount);

    std::optional<long long> balance(int account) const;
    std::size_t accounts() const;
    long long total() const;
    bool audit() const;
};
```

规则：

- `open` 创建余额为 0 的账户；编号已存在时返回 `false`，不改变状态。编号可以是负数。
- `deposit` 要求账户存在且 `amount > 0`，成功后余额增加。
- `withdraw` 要求账户存在、`amount > 0` 且余额足够；不足时不能透支。
- `transfer` 要求两个账户都存在、编号不同、`amount > 0`，且转出账户余额足够。
- 转账必须原子：任何条件不满足时，两个账户余额都不能改变；成功时转出减少、转入增加。
- 存款和取款会改变总资产；转账不改变总资产。
- `balance` 对未知账户返回 `std::nullopt`。
- `accounts` 返回账户数量；`total` 返回所有余额之和。
- `audit` 在当前账本一致（总资产等于各余额之和，且没有负余额）时返回 `true`。
- 不要写 `main`，不要修改公开接口。
