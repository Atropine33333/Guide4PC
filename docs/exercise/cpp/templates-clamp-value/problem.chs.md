《截断区间守则》

第一条 本区间只承认“小于”。尺子、字句、大象，凡能回答“谁比谁小”的，皆可入内。

第二条 遇 `value` 小于 `low`，交出 `low`；遇 `high` 小于 `value`，交出 `high`；两件事都没有发生，原样交出 `value`。

第三条 区间两端由调用者负责，`low` 不大于 `high`。此条为真，不必复核。

第四条 当 `low` 与 `high` 相等，区间只剩一个值。交出它。它既是左端，也是右端；常识在此不作数，守则作数。

第五条 本区间不办算术。加减乘除一概不受理，`<` 之外的门一概不开。要求 `T` 会做加法，属越权。

第六条 返回值类型为 `T`，按值交回。`T` 至少以 `int`、`double`、`char` 三种身份来访。

守则到此为止，各条均为真，请照办。

**人话**：实现函数模板 `clamp_value`，只使用 `<` 比较，把 `value` 截到 `[low, high]` 之内；`low <= high` 由调用者保证。把模板写成头文件式的实现。

```cpp
template <typename T>
T clamp_value(const T& value, const T& low, const T& high);
```

返回：

- 若 `value < low`，返回 `low`；
- 若 `high < value`，返回 `high`；
- 否则返回 `value`。

调用方保证 `low <= high`。`T` 至少会被实例化为 `int`、`double` 和 `char`。不要写 `main`。

要求：

- 只使用 `<` 比较，不要要求 `T` 支持算术运算；
- 当 `low == high` 时，区间内只剩一个值，结果必须是这个值；
- 返回值类型是 `T`（按值返回）。
