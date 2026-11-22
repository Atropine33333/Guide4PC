This exercise reads no stdin and writes no stdout. Implement a function that counts inversions; a hidden harness runs the tests.

```cpp
long long count_inversions(const std::vector<int>& values);
```

An inversion is a pair of indices `(i, j)` with `i < j` and `values[i] > values[j]`.

Rules:

- An empty sequence and a one-element sequence both give 0.
- Equal elements do not form an inversion; only strict greater-than counts.
- Values may be negative or duplicated; the length is at most 100000.
- The result may exceed the `int` range; the return type is `long long`.
- Do not write `main`.

### Hint

A double loop is too slow for 100000 elements. Count during a merge sort, or use a Fenwick tree with coordinate compression. During merging, taking an element from the right half makes it an inversion with every remaining element of the left half.
