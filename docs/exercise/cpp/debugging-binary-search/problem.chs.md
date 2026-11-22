（小剧场。桌上一把折扇，一块醒木。）

甲：今天给大伙说段二分。说有 `values` 一排数，从小到大排着，问头一个大于等于 `target` 的落在哪个下标。

乙：这还不简单？取中间一比，比 `target` 小就往右，不小就往左。

甲：听着挺顺耳。可人家点名要“头一个”，头一个后头的那些位置，一律不作数。

乙：差在哪儿？

甲：`values[mid]` 正好等于 `target` 的时候，您把 `low` 推到 `mid + 1`，头一个等于它的位置就这么让过去了，末了报上来的是头一个“大于”它的。

乙：那我改。相等的时候往左收，把 `high` 收到 `mid`，区间里照样留着它。

甲：对了。收到底，`low` 就是头一个大于等于它的位置。

乙：全都比它小呢？

甲：那就停在 `values.size()`，一个也不剩。数组是空的也一样，返回 0。

乙：得嘞，题面我给您念念。

**人话**：实现 `lower_bound_index(values, target)`：在非降序数组中返回第一个满足 `values[i] >= target` 的下标；所有元素都小于 `target` 时返回 `values.size()`。不读 stdin、不写 stdout，隐藏 harness 直接调用。starter 在相等时把 `low` 推到 `mid + 1`，改用收窄右边界、保留 `mid` 的写法即可，函数签名保持原样。

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

提示：当前实现在 `values[mid] == target` 时会把 `low` 推到 `mid + 1`，于是返回的是第一个大于 `target` 的位置；题目要的是第一个大于等于它的位置。
