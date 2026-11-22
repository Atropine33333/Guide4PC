（占卜师取出一副牌，牌面写的全是类型名，一张也不落到桌上。）

问卜者：这副牌不发给手，只在台下排着？

占卜师：正是。`TypeList<Ts...>` 只在编译期存在，牌不占人间一寸地方。

问卜者：牌数怎么算？

占卜师：`size`，`sizeof...(Ts)`。空牌堆是 0。

问卜者：头一张呢？

占卜师：`front`。空牌堆的 `front` 记作 `void`，那一张没人会去翻。

问卜者：新牌压在最上面呢？

占卜师：`push_front<T>`，得 `TypeList<T, Ts...>`。

问卜者：我要查某张牌在不在堆里？

占卜师：`contains<T>`，在就是 `true`，不在就是 `false`。同一张牌出现两次，也只算在。

问卜者：第 `Index` 张怎么取？

占卜师：`at<Index>`，从 0 数起；只问堆里真有的位置。

问卜者：这些牌，运行时还看得见么？

占卜师：看不见。牌面就是这些类型，答案在编译期就定下了。

**人话**：实现只在编译期存在的 `TypeList<Ts...>`，提供 `size`、`front`、`push_front`、`contains`、`at`，全部结果在编译期可用。

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
