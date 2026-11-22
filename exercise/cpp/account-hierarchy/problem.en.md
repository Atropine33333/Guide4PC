*An Ordinance on Bank Accounts*

Article 1 (Purpose). To fix the bounds of deposit and withdrawal for current and savings accounts, this ordinance is enacted.

Article 2 (The Account). Let `Account` be the base class: `explicit Account(int balance)` records a non-negative opening balance; its destructor is virtual; `int balance() const` reports the balance.

Article 3 (Deposits). `bool deposit(int amount)`: an amount greater than zero is credited and answered with `true`; every other amount is refused with `false`, the ledger untouched.

Article 4 (Withdrawals). Every withdrawal requires an amount greater than zero, and a failed one leaves the balance unchanged. A current account pays out whenever the balance suffices; a savings account must still hold at least `1000` afterwards, or the request is refused. Withdrawal is the pure virtual `bool withdraw(int amount)`.

Article 5 (Names). `const char* kind() const` is pure virtual: `"checking"` for current, `"savings"` for savings.

Article 6 (Execution). `withdraw` and `kind` are called through `Account*`, so both must dispatch virtually.

```cpp
class Account {
public:
    explicit Account(int balance);
    virtual ~Account();

    bool deposit(int amount);
    virtual bool withdraw(int amount) = 0;
    int balance() const;
    virtual const char* kind() const = 0;
};

class Checking : public Account {
public:
    explicit Checking(int balance);
    bool withdraw(int amount) override;
    const char* kind() const override;
};

class Savings : public Account {
public:
    explicit Savings(int balance);
    bool withdraw(int amount) override;
    const char* kind() const override;
};
```

**In plain words**: Implement the base class `Account` and the derived classes `Checking` and `Savings`: deposits accept positive amounts only, withdrawals follow each account's own rule and leave the balance unchanged when refused, and `kind()` reports the account name.
