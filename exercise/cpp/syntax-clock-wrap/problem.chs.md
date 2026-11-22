这一题不读 stdin，也不写 stdout。实现一个会自动跨午夜回绕的时钟，测试在隐藏的 harness 里。

实现 `Clock`：

- `Clock(int hour, int minute, int second)`：三个分量可以是任意整数（包括负数）；构造后必须归一化到 `00:00:00`–`23:59:59`。
- `void advance(int seconds)`：向前走 `seconds` 秒；`seconds` 可以为负。
- `int hour() const`、`int minute() const`、`int second() const`：返回归一化后的分量。
- `int total_seconds() const`：返回从 `00:00:00` 起经过的秒数，范围是 `[0, 86399]`。
- `bool operator==(const Clock& other) const`：表示同一时刻时返回 `true`。

所有运算都按 24 小时循环进行。例如 `-1` 秒是 `23:59:59`，`25:00:00` 是次日 `01:00:00`。不要写 `main`。
