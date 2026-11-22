This exercise reads no stdin. Implement a histogram that buckets values by a fixed width.

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

Rules:

- Both constructor arguments `buckets` and `width` are positive.
- A non-negative `value` belongs to bucket `value / width` (integer division).
- If the bucket index is at least `buckets`, the value counts as overflow and enters no bucket.
- Negative values are ignored: they change neither bucket counts nor `total` nor `overflow`.
- `total` is the number of counted values, equal to the sum of all bucket counts plus `overflow`.
- `count(bucket)` returns the count of bucket `bucket`; for an out-of-range index (including negative) it returns `0`.
- Do not write `main`.

### Example

`Histogram h(4, 10)` fed `0, 9, 10, 35, 39, 40` has bucket counts `2, 1, 0, 2`, `overflow` `1`, and `total` `6`.

### Hint

Keep bucket counts in an `int` array (or `std::vector<int>`), plus separate `total_` and `overflow_` counters.
