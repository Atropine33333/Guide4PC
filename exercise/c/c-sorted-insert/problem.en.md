# Into the Line

**In plain words**: maintain an array of integers kept in ascending order. Each insert puts a value in the right place so the array stays non-decreasing; the array holds at most 64 values.

**Interface**:

```c
#define ARR_CAP 64

typedef struct {
    int data[ARR_CAP];
    int size;
} SortedArray;

void sa_init(SortedArray *a);
int sa_insert(SortedArray *a, int value);
int sa_size(const SortedArray *a);
```

- `sa_init` empties the array.
- `sa_insert` returns `0` on success; with all 64 slots taken it returns `-1` and leaves the array unchanged.
- `sa_size` returns the number of elements.

### Example

After inserting `5`, `1` and `4`, `data[0..2]` reads `1 4 5` and `sa_size` is `3`.

### Contract

- Implement only these functions; do not write `main`.
- No dynamic allocation; the capacity is `ARR_CAP`.
- `data[0]` through `data[size - 1]` is always non-decreasing.
- Equal values are all kept; their relative order is free.
- The contents of unused slots do not matter.
