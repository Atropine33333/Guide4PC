This task reads no stdin and writes no stdout. Implement a ledger with atomic transfers.

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

Rules:

- `open` creates an account with balance 0; return `false` and change nothing if the id already exists. Account ids may be negative.
- `deposit` requires an existing account and `amount > 0`; on success, increase the balance.
- `withdraw` requires an existing account, `amount > 0`, and sufficient funds; never allow an overdraft.
- `transfer` requires both accounts, distinct ids, `amount > 0`, and sufficient funds in the source account.
- Transfers must be atomic: if any condition fails, neither balance changes. On success, debit the source and credit the destination.
- Deposits and withdrawals change total assets; transfers do not.
- `balance` returns `std::nullopt` for an unknown account.
- `accounts` returns the number of accounts; `total` returns the sum of all balances.
- `audit` returns `true` when the ledger is consistent (total assets equal the sum of balances and no balance is negative).
- Do not write `main`, and do not change the public interface.
