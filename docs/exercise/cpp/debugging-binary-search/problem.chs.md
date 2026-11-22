这一题不读 stdin，也不写 stdout。下面是一个有错误的二分查找实现；请修正它，使隐藏的 harness 通过。

函数：

```cpp
int lower_bound_index(const std::vector<int>& values, int target);
```

返回第一个满足 `values[i] >= target` 的下标 `i`。如果不存在这样的下标（即所有元素都小于 `target`），返回 `values.size()`。

约束：

- `values` 按非降序排列，可能为空，可能包含重复值；
- 返回值是 `int`，范围 `0..values.size()`；
- 不要写 `main`；
- 只能修 `starter.cpp` 中的逻辑，不要改变函数签名。

提示：当前实现在 `values[mid] == target` 时会把 `low` 推到 `mid + 1`，于是返回的是第一个大于 `target` 的位置，而不是第一个大于等于它的位置。
