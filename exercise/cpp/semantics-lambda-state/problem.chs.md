用 lambda 实现两个返回闭包的工厂函数。

```cpp
std::function<int()> make_counter(int start);
std::function<int(int)> make_accumulator(int start);
```

规则：

- `make_counter(start)` 返回一个计数器：每次调用先把内部计数加 1，再返回新的值；第一次调用返回 `start + 1`。
- `make_accumulator(start)` 返回一个累加器：每次调用把参数加到内部总和上，并返回新的总和；第一次调用返回 `start + 参数`。
- 闭包的状态按值捕获，两个闭包之间互不影响。
- 拷贝一个 `std::function` 会拷贝闭包当前的状态；之后原闭包和副本各自独立递增。
- `start` 和参数都可能是负数或 0。
- 不要写 `main`。

### 提示

在捕获列表里用初始化捕获保存状态，并把 lambda 标记为 `mutable`，否则不能修改按值捕获的变量。
