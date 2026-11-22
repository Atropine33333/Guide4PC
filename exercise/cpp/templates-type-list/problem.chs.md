这一题不读 stdin，也不写 stdout。实现一个只存在于编译期的类型列表。

```cpp
#include <cstddef>

template <typename... Ts>
struct TypeList {
    static constexpr std::size_t size = sizeof...(Ts);

    using front = /* 第一个类型 */;
    template <typename T>
    using push_front = /* 新列表 */;

    template <typename T>
    static constexpr bool contains = /* 是否包含 T */;

    template <std::size_t Index>
    using at = /* 第 Index 个类型 */;
};
```

规则：

- `size` 是类型个数，空列表为 0。
- `front` 是第一个类型；空列表的 `front` 定义为 `void`（测试不会取空列表的 front）。
- `TypeList<Ts...>::push_front<T>` 产生 `TypeList<T, Ts...>`。
- `contains<T>` 是编译期布尔值，空列表为 `false`，重复类型也只关心是否存在。
- `at<Index>` 是第 `Index` 个类型，下标从 0 开始；测试只访问合法下标。
- 所有结果都必须在编译期可用，不能依赖运行时容器。
- 不要写 `main`，不要修改公开接口。
