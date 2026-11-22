《Guide 银行账户条例》

第一条（宗旨）为厘清活期与储蓄两类账户的存取界限，制定本条例。

第二条（账户）设基类 `Account`：`explicit Account(int balance)` 记下非负的初始余额，析构函数为虚函数，`int balance() const` 报出余额。

第三条（存款）`bool deposit(int amount)`：金额大于零者入账并报 `true`；其余情形驳回，报 `false`，账面分文不动。

第四条（取款）一切取款皆要求金额大于零，失败时余额不变。活期账户余额足够即可支取；储蓄账户支取之后，余额仍须不少于 `1000`，否则驳回。取款为纯虚函数 `bool withdraw(int amount)`。

第五条（名目）`const char* kind() const` 为纯虚函数：活期报 `"checking"`，储蓄报 `"savings"`。

第六条（施行）`withdraw` 与 `kind` 经 `Account*` 调用，故二者须经虚函数分派。

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

**人话**：实现账户基类 `Account` 与派生类 `Checking`、`Savings`：存款只接受正数，取款按各账户的规矩判断，失败时余额保持不变；`kind()` 报出账户名目。
