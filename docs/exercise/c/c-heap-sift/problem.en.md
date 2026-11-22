The orbital freight depot stacks crates into an energy pyramid: the lightest crate always sits at the apex, and no bay is heavier than the two bays beneath it. Cargo loads from the bottom; when the base shifts, the apex crate sinks along the lighter side until it settles.

**In plain words**: maintain a set of integers in a min-heap — `heap_build` turns an array into a min-heap, `heap_sift_down` lets the element at some position sink by swapping with its smaller child, and `heap_extract_min` takes away the smallest one each time.

**Interface**:

```c
#include <stddef.h>

#define HEAP_CAP 64

typedef struct {
    int data[HEAP_CAP];
    size_t len;
} MinHeap;

void heap_sift_down(MinHeap *h, size_t i);
void heap_build(MinHeap *h, const int *values, size_t n);
int heap_extract_min(MinHeap *h, int *out);
```

- Position `i` has children `2 * i + 1` and `2 * i + 2`, and its parent is `(i - 1) / 2`; a min-heap requires every child to be no smaller than its parent.
- `heap_build` copies the first `n` values into `h->data`, sets `h->len = n`, then sifts down from the last non-leaf position back to the root; for `n == 0`, `h->len` is `0`.
- `heap_sift_down` assumes both subtrees are already valid min-heaps and sinks the element at `i` by swapping it with its smaller child until it settles.
- `heap_extract_min` returns `-1` on an empty heap and leaves `*out` alone; otherwise it writes the root into `*out`, moves the last element to the root, sifts down once, decreases `len` by one and returns `0`.

### Example

Building from `5 3 8 1 9 2 7` and extracting seven times yields `1 2 3 5 7 8 9` in that order; the next extraction returns `-1` with `len` now `0`.

### Contract

- Implement only these functions; do not write `main`.
- The element count never exceeds `HEAP_CAP`, and the array is stored level by level.
- Duplicate values are stored as usual; extractions only need to be non-decreasing.
- Keep the struct fields as given and the function signatures unchanged.
