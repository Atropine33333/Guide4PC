实现一个按固定批大小收集整数的累加器。

```cpp
#include <cstddef>
#include <vector>

class BatchAccumulator {
public:
    explicit BatchAccumulator(std::size_t batch_size);

    bool add(int value);
    std::vector<int> take_batch();
    std::vector<int> flush();

    std::size_t pending() const;
    std::size_t emitted() const;
};
```

规则：

- `batch_size` 小于 1 时按 1 处理。
- `add` 在当前待处理数量小于批大小时加入并返回 `true`；已经攒满一批时返回 `false`，不加入。
- `take_batch` 仅在待处理数量恰好等于批大小时返回整批并按加入顺序清空；否则返回空 `vector`，不改变状态。
- `flush` 返回当前所有待处理元素（可以为不满一批，也可以为空）并清空。
- `pending()` 返回当前待处理数量。
- `emitted()` 返回已经由 `take_batch` 和 `flush` 返回过的元素总数（只统计真正返回的元素）。
- 不要写 `main`，不要修改公开接口。
