这一题不读 stdin，也不写 stdout。实现一个账户继承层次，测试在隐藏的 harness 里。

基类 `Account`：

- `explicit Account(int balance)`：初始余额，非负。
- 虚析构函数。
- `bool deposit(int amount)`：`amount` 必须大于 0，成功时增加余额并返回 `true`；否则返回 `false` 且余额不变。
- 纯虚函数 `bool withdraw(int amount)`。
- `int balance() const`。
- 纯虚函数 `const char* kind() const`。

派生类：

- `Checking`（活期）：只要余额足够就能取款。`kind()` 返回 `"checking"`。
- `Savings`（储蓄）：取款后余额必须仍然不少于 `1000`，否则拒绝。`kind()` 返回 `"savings"`。

所有取款都要求 `amount > 0`，且失败时余额不变。harness 会通过 `Account*` 调用 `withdraw`，所以它必须是虚函数。
