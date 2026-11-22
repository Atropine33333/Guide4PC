实现一个对元组每个元素应用函数的模板。

```cpp
#include <tuple>
#include <utility>

template <typename Tuple, typename F>
auto tuple_map(Tuple&& tuple, F&& f);
```

规则：

- 返回一个 `std::tuple`，第 `i` 个元素是 `f(std::get<i>(tuple))`。
- 必须保持元素顺序和数量；结果元素的类型由 `f` 的返回类型决定。
- 支持空元组：返回空元组，不调用 `f`。
- 支持 `const` 元组，且不修改输入。
- 支持函数指针、lambda 和带重载 `operator()` 的函数对象。
- 建议使用 `std::index_sequence`、`std::make_index_sequence` 和折叠表达式/包展开。
- 不要写 `main`，不要修改函数签名。
