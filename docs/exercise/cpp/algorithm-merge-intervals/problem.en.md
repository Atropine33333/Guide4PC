**Chapter Seven**

The children of Israel went out of Egypt and pitched in the wilderness, tribe by tribe. Each tribe pitched from `left` to `right`; where two camps touched, their tents became one camp.

And the LORD spake unto Moses, saying: "Number the camps of the tribes. Every camp that touches another shall be joined into one; those that merely stand side by side, not touching, shall each keep its own ground. When the joining is done, write them down from the smallest starting place to the greatest. Leave out no tribe, and join no camp twice."

And Moses did as the LORD commanded.

**In plain words**: implement `merge_intervals`, which merges a collection of closed intervals into a list of disjoint intervals sorted by left endpoint. Nothing is read from stdin and nothing is written to stdout; a hidden harness runs the tests, so do not write `main`.

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
