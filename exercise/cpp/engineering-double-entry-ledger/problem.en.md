The kingdom of heaven is like a counting house. Before leaving on a long journey, the master handed the ledger to his servant.

The master said: when you open an account for anyone, write zero beside the name. If the same name is opened a second time, do not write it, and do not move the books.

The servant asked about deposits and withdrawals. The master said: the amount must be greater than zero, the account must already exist, and for a withdrawal the balance must suffice. If it falls short, not one coin may move.

Another servant kept the transfers. The master commanded: source and destination must both exist, the two names must differ, the amount must be greater than zero, and the source must hold enough. Success and failure happen in one instant: if a single condition fails, neither balance may move.

At the end the master said: deposits and withdrawals change the total; transfers change it not. If a name is not in the ledger, answer "none" when its balance is asked. When the books are audited, they are clean only when the total equals the sum of all balances and no balance is negative.

**In plain words**: Complete `Ledger`. Balances start at zero and account ids may be negative; `transfer` either moves both sides or moves nothing; `total()` is the sum of all balances, and `audit()` returns `true` when the total equals the sum of balances and no balance is negative. Do not write `main`, and do not change the public interface.

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

### Rules

- `open` creates an account with balance 0; return `false` and change nothing if the id already exists.
- `deposit` requires an existing account and `amount > 0`; on success, increase the balance.
- `withdraw` requires an existing account, `amount > 0`, and sufficient funds; never allow an overdraft.
- `transfer` requires both accounts, distinct ids, `amount > 0`, and sufficient funds in the source account. If any condition fails, neither balance changes; on success, debit the source and credit the destination.
- Deposits and withdrawals change total assets; transfers do not.
- `balance` returns `std::nullopt` for an unknown account; `accounts` returns the number of accounts; `total` returns the sum of all balances.
- `audit` returns `true` when the ledger is consistent (total assets equal the sum of balances and no balance is negative).
- Do not write `main`, and do not change the public interface.
