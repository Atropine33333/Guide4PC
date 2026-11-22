class Account {
public:
    explicit Account(int balance) : balance_(balance) {}

    virtual ~Account() = default;

    bool deposit(int amount) {
        if (amount <= 0) return false;
        balance_ += amount;
        return true;
    }

    virtual bool withdraw(int amount) = 0;

    int balance() const { return balance_; }

    virtual const char* kind() const = 0;

protected:
    int balance_ = 0;
};

class Checking : public Account {
public:
    explicit Checking(int balance) : Account(balance) {}

    bool withdraw(int amount) override {
        (void)amount;
        // TODO: 余额足够时扣款并返回 true。
        return false;
    }

    const char* kind() const override { return "checking"; }
};

class Savings : public Account {
public:
    explicit Savings(int balance) : Account(balance) {}

    bool withdraw(int amount) override {
        (void)amount;
        // TODO: 扣款后余额不得低于 1000。
        return false;
    }

    const char* kind() const override { return "savings"; }
};
