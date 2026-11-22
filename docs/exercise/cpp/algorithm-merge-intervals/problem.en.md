This exercise reads no stdin and writes no stdout. Implement a function that merges closed intervals; a hidden harness runs the tests.

First define the struct:

```cpp
struct Interval {
    int left;
    int right;
};
```

Implement:

- `std::vector<Interval> merge_intervals(std::vector<Interval> intervals)`: merge all intersecting closed intervals and return the resulting list sorted by `left` with no overlaps.

After sorting, closed intervals `[a, b]` and `[c, d]` intersect exactly when `c ≤ b`. Intervals that share an endpoint, such as `[1, 2]` and `[2, 3]`, must merge; merely adjacent intervals, such as `[1, 2]` and `[3, 4]`, must not. Every input interval satisfies `left ≤ right`. Do not write `main`.
