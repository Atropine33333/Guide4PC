天国好像一个账房。主人出远门以前，把账簿交给仆人。

主人说：给谁开户，就给他记上零。同一个名字开第二回，不要记，账也不要动。

仆人问：存入与取出呢？主人说：数目要大过零，账户得先有；取出的，账上还得够。不够的，一两也不许动。

管转账的仆人又问。主人吩咐：转出与转入都得有户头，两个名字不能相同，数目要大过零，转出那边得够数。成与不成只在一瞬间：条件差一条，两边的数目都不许动。

主人末了说：存入与取出会让总账变，转账不会。谁的户头不在簿上，问余额就答“没有”。查账的时候，总资产等于各余额之和、且没有负数余额，才算账目干净。

**人话**：补完 `Ledger`。账户余额从零起步，编号可以为负；`transfer` 要么两边都成、要么两边都不动；`total()` 是全部余额之和，`audit()` 在总资产等于各余额之和且没有负余额时返回 `true`。不要写 `main`，公开接口不许改。

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

### 规则或约定

- `open` 创建余额为 0 的账户；编号已存在时返回 `false`，不改变状态。
- `deposit` 要求账户存在且 `amount > 0`，成功后余额增加。
- `withdraw` 要求账户存在、`amount > 0` 且余额足够，不许透支。
- `transfer` 要求两个账户都存在、编号不同、`amount > 0`，且转出账户余额足够；任何条件不满足时两边余额都不变，成功时转出减少、转入增加。
- 存款和取款会改变总资产，转账不改变总资产。
- `balance` 对未知账户返回 `std::nullopt`；`accounts` 返回账户数量；`total` 返回所有余额之和。
- `audit` 在当前账本一致（总资产等于各余额之和，且没有负余额）时返回 `true`。
- 不要写 `main`，不要修改公开接口。
