这一题不读 stdin，也不写 stdout。下面的 `ScoreBoard` 能编译，但少数行为不符合约定。请找出并修复 bug，测试在隐藏的 harness 里。

约定的行为：

- `void add(const std::string& player, int points)`：给玩家加分。同一个玩家多次加分应当**累加**。`points` 为正数。
- `bool score(const std::string& player, int& out) const`：玩家存在时把总分写入 `out` 并返回 `true`，否则返回 `false`。
- `int players() const`：返回出现过的玩家数量。
- `bool leader(std::string& out) const`：把总分最高的玩家名字写入 `out` 并返回 `true`；总分并列时选字典序最小的名字；记分板为空时返回 `false`。

只修改 `ScoreBoard` 的实现，不要改变成员函数的签名，也不要写 `main`。
