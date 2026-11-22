这一题不读 stdin，也不写 stdout。实现一个发号器，测试在隐藏的 harness 里。

实现三个函数：

- `int next_ticket()`：返回下一个票号，从 1 开始，每调用一次加 1。
- `int tickets_issued()`：返回自上次重置以来已经发出的票数。
- `void reset_tickets()`：重置发号器；下一次 `next_ticket()` 仍然返回 1，`tickets_issued()` 返回 0。

状态必须在多次函数调用之间保留。不要写 `main`。
