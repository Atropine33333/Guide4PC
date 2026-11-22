实现一个闭区间类。

```cpp
class Interval {
public:
    Interval(int low, int high);
    int low() const;
    int high() const;
    bool contains(int value) const;
    bool overlaps(const Interval& other) const;
    Interval united(const Interval& other) const;
    bool operator==(const Interval& other) const;
};
```

规则：

- 构造时如果 `low > high`，交换两个端点，保证 `low() <= high()`。
- 区间是闭区间：`contains` 在端点上也返回 `true`。
- `overlaps` 判断两个闭区间是否有公共点；仅仅端点相接（`a.high() == b.low()`）也算重叠。
- `united` 返回能同时包含两个区间的最小闭区间（即使它们不相交，也返回中间的空隙一起包住的区间）。
- `operator==` 当且仅当两个区间的两个端点都相等时返回 `true`。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `Interval(5, 2).low()` / `.high()` | `2` / `5` |
| `Interval(1, 4).contains(4)` | `true` |
| `Interval(1, 2).overlaps(Interval(2, 5))` | `true` |
| `Interval(1, 3).united(Interval(5, 7))` | `[1, 7]` |

### 提示

构造函数用成员初始化列表和 `std::min`/`std::max` 或条件交换。`overlaps` 可以写成 `!(high_ < other.low_ || other.high_ < low_)`。`united` 返回 `Interval(std::min(low_, other.low_), std::max(high_, other.high_))`。
