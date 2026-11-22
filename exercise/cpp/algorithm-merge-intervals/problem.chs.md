这一题不读 stdin，也不写 stdout。实现一个合并闭区间的函数，测试在隐藏的 harness 里。

先定义结构体：

```cpp
struct Interval {
    int left;
    int right;
};
```

实现：

- `std::vector<Interval> merge_intervals(std::vector<Interval> intervals)`：合并所有相交的闭区间，返回按 `left` 升序排列、互不相交的区间列表。

两个闭区间 `[a, b]` 和 `[c, d]` 相交当且仅当 `c ≤ b`（排序后）。端点相接的区间 `[1, 2]` 与 `[2, 3]` 要合并；仅仅相邻的 `[1, 2]` 与 `[3, 4]` 不合并。输入中的每个区间都满足 `left ≤ right`。不要写 `main`。
