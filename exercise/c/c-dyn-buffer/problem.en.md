Tired of running out of room? The next generation of self-stretching cloth has arrived!

Just keep stitching: the first stitch starts palm-sized, and it grows longer as you go, doubling whenever it runs short. The seams stay flat, and a quiet `\0` always sits at the end. One shake when you are done, and `buf_free` takes the whole cloth away without leaving a thread; stitch again later and it is brand new.

**In plain words**: `Buf` is a character buffer that grows on demand. `buf_init` empties it; `buf_append` appends a whole C string, growing the storage when needed, and guarantees that `p` is `\0`-terminated and `cap` is at least `len + 1`; `buf_free` releases the memory and zeroes `p`, `len` and `cap`.

**Interface**:

```c
#include <stddef.h>

typedef struct {
    char *p;
    size_t len;
    size_t cap;
} Buf;

void buf_init(Buf *b);
int buf_append(Buf *b, const char *s);
void buf_free(Buf *b);
```

- After `buf_init`: `p == NULL`, `len == 0`, `cap == 0`.
- `buf_append` returns `0` on success; `len` grows by the length of the appended string, `p[0..len-1]` holds the content and `p[len]` is `'\0'`. It returns `-1` when memory runs out and leaves the buffer unchanged.
- `buf_free` releases the memory and zeroes the three fields; calling it on a freshly initialised empty buffer is safe.

### Example

```c
Buf b;
buf_init(&b);
buf_append(&b, "ab");
buf_append(&b, "cde");
/* now b.len == 5, b.p is "abcde", b.cap >= 6 */
buf_free(&b);
/* now b.p == NULL, b.len == 0, b.cap == 0 */
```

### Contract

- Implement only these functions; do not write `main`.
- Any growth strategy is fine as long as `cap >= len + 1` and a failed allocation returns `-1`.
- Appending an empty string changes nothing.
- Keep the struct fields as given and the function signatures unchanged.
