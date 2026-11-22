这一题不读 stdin，也不写 stdout。实现一个返回函数组合的模板。

```cpp
#include <utility>

template <typename F, typename G>
auto compose(F f, G g);
```

返回的可调用对象 `h` 满足：对任意单个参数 `x`，有

```cpp
h(x) == f(g(x))
```

即先调用 `g`，再把结果传给 `f`。

规则：

- 支持普通函数指针、lambda、带 `const operator()` 的函数对象。
- 返回对象按值保存 `f` 和 `g`，可以重复调用，也可以继续参与嵌套组合。
- `f` 和 `g` 的返回类型可以不同；`compose` 的返回类型由编译器推导。
- 不要使用 `std::function` 作为返回类型（允许在测试中传入它）。
- 不要写 `main`，不要修改函数签名。
