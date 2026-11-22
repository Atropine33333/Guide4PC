实现一个按固定宽度分桶的直方图。

```cpp
class Histogram {
public:
    Histogram(int buckets, int width);
    void add(int value);
    int buckets() const;
    int width() const;
    int total() const;
    int overflow() const;
    int count(int bucket) const;
};
```

约定：

- 构造参数 `buckets` 和 `width` 都是正整数。
- 对非负的 `value`，它属于第 `value / width` 个桶（整数除法）。
- 如果桶编号大于等于 `buckets`，该值计入溢出（`overflow`），不进入任何桶。
- 负值被忽略：既不增加桶计数，也不增加 `total` 或 `overflow`。
- `total` 返回被统计的数值个数，等于所有桶计数之和加上 `overflow`。
- `count(bucket)` 返回第 `bucket` 个桶的计数；`bucket` 越界（包括负数）时返回 `0`。
- 不要写 `main`。

### 示例

`Histogram h(4, 10)` 依次加入 `0, 9, 10, 35, 39, 40`：四个桶的计数是 `2, 1, 0, 2`，`overflow` 为 `1`，`total` 为 `6`。

### 提示

用一个 `int` 数组（或 `std::vector<int>`）保存桶计数，另外维护 `total_` 和 `overflow_` 两个计数器。
