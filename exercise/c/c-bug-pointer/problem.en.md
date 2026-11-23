Two functions in `starter.c` deal with pointers, and both are wrong.

`swap_ints` should exchange two integers, but neither value moves. `max_ptr` should return a pointer to the largest element of an array, but it always points at the last element: it compares addresses and never compares the values stored at them.

Fix both spots.

**In plain words**: `swap_ints` must exchange the two `int`s the pointers refer to; `max_ptr` must find the largest value in the array and return a pointer to it, choosing the first occurrence when the maximum repeats.

**Interface**:

```c
void swap_ints(int *a, int *b);
int *max_ptr(int *a, int n);
```

- `swap_ints` exchanges `*a` and `*b`. When both pointers refer to the same variable, that variable keeps its value.
- `max_ptr` returns a pointer to the largest element among `a[0]` to `a[n-1]`; when the maximum occurs more than once, it returns the first such position.
- `n` is between `1` and `1000`, and `a` points at least `n` valid `int`s.

### Example

```c
int x = 3;
int y = 7;
swap_ints(&x, &y);           /* x is 7, y is 3 */

int data[4] = {2, 9, 4, 9};
int *p = max_ptr(data, 4);   /* p points at data[1] */
```

### Contract

- Change only these two functions; do not write `main`.
- Every pointer passed in refers to a valid `int`; null pointers need no handling.
- Write the results back through the pointers: `swap_ints` returns `void`, so the exchanged values can only land in `*a` and `*b`.
