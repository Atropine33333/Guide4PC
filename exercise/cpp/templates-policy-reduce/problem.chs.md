这一题不读 stdin。用策略类实现一个可复用的区间归约函数。

```cpp
#include <vector>

struct SumPolicy {
    static int combine(int left, int right);
    static int identity();
};

struct MaxPolicy {
    static int combine(int left, int right);
    static int identity();
};

template <class Policy>
int reduce(const std::vector<int>& values);
```

约定：

- `SumPolicy` 的 `identity()` 返回 `0`，`combine` 返回两数之和。
- `MaxPolicy` 的 `identity()` 返回 `INT_MIN`，`combine` 返回较大的那个数。
- `reduce<Policy>` 以 `Policy::identity()` 为初值，从左到右依次执行 `acc = Policy::combine(acc, value)`。
- 空序列返回 `Policy::identity()`。
- 不要写 `main`。

harness 还会定义自己的策略类（例如乘法策略）并调用 `reduce<该策略>`，所以不要把逻辑写死成求和或求最大值。

### 提示

`reduce` 定义在头文件里，模板参数只需要提供 `identity()` 与 `combine(int, int)` 两个静态成员。
