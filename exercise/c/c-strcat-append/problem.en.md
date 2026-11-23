Two pieces of text: one already sits in a buffer, the other is to be attached to its tail. The buffer has a fixed number of bytes; if the tail fits, attach it, and if it does not, leave the buffer as it was.

**In plain words**: implement `append_str(buf, cap, s)` to attach the string `s` to the end of `buf`. When the capacity allows it, append and return `0`, keeping the closing `'\0'`; when it does not, return `-1` and leave `buf` untouched.

**Interface**:

```c
#include <stddef.h>

int append_str(char *buf, size_t cap, const char *s);
```

- `buf` already holds a `'\0'`-terminated string, and `cap` is the total size of the buffer in bytes, at least `1`.
- `s` is a `'\0'`-terminated string and may be empty.
- If it fits (`strlen(buf) + strlen(s) + 1 <= cap`): attach `s` at the end, keep the result `'\0'`-terminated, and return `0`.
- If it does not fit: leave `buf` exactly as it was and return `-1`.

### Example

With `char buf[10] = "cat";`, calling `append_str(buf, 10, "s")` returns `0` and `buf` becomes `"cats"`.

With `char buf[4] = "cat";`, calling `append_str(buf, 4, "s")` returns `-1` and `buf` stays `"cat"`: three characters plus the closing `'\0'` plus one `s` need `5` bytes.

### Contract

- Implement only this function; do not write `main`.
- Measure both pieces of text with `strlen`.
- On success the result must be `'\0'`-terminated; on failure the buffer keeps its contents.
- Print nothing at all.
