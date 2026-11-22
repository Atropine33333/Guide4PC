This exercise reads no stdin and writes no stdout. Implement an account hierarchy; a hidden harness runs the tests.

Base class `Account`:

- `explicit Account(int balance)`: the initial balance, non-negative.
- A virtual destructor.
- `bool deposit(int amount)`: `amount` must be greater than 0; on success add it and return `true`, otherwise return `false` with the balance unchanged.
- Pure virtual `bool withdraw(int amount)`.
- `int balance() const`.
- Pure virtual `const char* kind() const`.

Derived classes:

- `Checking`: withdrawals succeed whenever the balance is sufficient. `kind()` returns `"checking"`.
- `Savings`: the balance must remain at least `1000` after the withdrawal, otherwise it is rejected. `kind()` returns `"savings"`.

Every withdrawal requires `amount > 0` and leaves the balance unchanged on failure. The harness calls `withdraw` through `Account*`, so it must be virtual.
