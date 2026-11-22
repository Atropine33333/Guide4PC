(2 a.m. The shopping channel. A host in a sequined blazer holds up a clear plastic box. The crowd goes wild.)

"Folks, look at this box! Empty right now. Drop one integer in. Another. Another—"

(Ding!)

"Full? The word isn't in this box's dictionary! When capacity runs out, the box doubles itself. Every integer stays, in order, not one lost."

"Done with it? Press `vec_free`. The box folds up clean, `size` back to zero."

"What if growing fails? `vec_push` honestly returns `-1`, and the data already inside stays untouched."

"Call 676-6767 now! But wait—there's more. The first ten callers get a free `cap` counter that tracks the capacity wherever it grows!"

"Operators are standing by."

**In plain words**: implement the three functions of `IntVec`. `vec_init` makes it empty; `vec_push` appends an integer, growing the capacity when needed (the new capacity is at least twice the old one), returning `0` on success and `-1` on failure with the existing data untouched; `vec_free` releases the memory and sets `size` to `0`.

**Interface**:

```c
typedef struct {
    int *data;
    size_t size;
    size_t cap;
} IntVec;

void vec_init(IntVec *v);
int vec_push(IntVec *v, int value);
void vec_free(IntVec *v);
```

### Example

| Call | Result |
| --- | --- |
| `vec_init(&v)` | `size == 0`, `cap == 0`, `data == NULL` |
| `vec_push(&v, 7)`, `vec_push(&v, 9)` | both return `0`; `size == 2`, `data[0] == 7`, `data[1] == 9` |
| then push 98 more integers | all return `0`; `size == 100`, `cap >= 100`, values in insertion order |
| `vec_free(&v)` | memory released, `size == 0` |

### Contract

- After `vec_init`: `size == 0`, `cap == 0`, `data == NULL`.
- The first growth from capacity `0` allocates at least `1` slot.
- Whenever the vector grows, the new capacity is at least twice the old one; `cap >= size` always holds.
- Old elements keep their order across a growth; none may be lost.
- When allocation fails, `vec_push` returns `-1` and leaves `size` and the existing elements unchanged.
- `vec_free` is safe even when nothing was ever pushed; afterwards `size == 0`. Free a given `IntVec` only once.
- Memory comes from `malloc`/`realloc`, is owned by the `IntVec`, and is returned by `vec_free`.
- Do not write `main`.
