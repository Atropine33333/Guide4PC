Copy a source string into a destination buffer, writing no further than the capacity allows, and finish with a `\0`. When the destination has limited room and the source length is unknown, no source string, however long, will be written past the destination buffer, and the result is always a valid C string.

**In plain words**: copy `s` into `dst`, writing at most `cap - 1` characters and then a `\0`; when `cap` is `0`, write not a single byte.

**Interface**:

```c
void copy_name(char *dst, size_t cap, const char *s);
```

- `cap` is `0`: do nothing, not even the closing `\0`; leave `dst` as it was.
- `cap` greater than `0`: write at most `cap - 1` characters, then a `\0`; extra source characters are dropped without error.
- When `cap` fits the whole string, the effect matches a full copy.
- `cap` counts bytes and includes the slot for the closing `\0`; the byte at index `cap` and everything after it in the destination are untouched; the source string itself is unaffected.

### Example

```c
char a[16];
copy_name(a, 8, "0123456");   // a becomes "0123456"

char b[16];
copy_name(b, 5, "abcdefgh");  // b becomes "abcd"

char c[8];
copy_name(c, 1, "abc");       // c becomes ""
```

### Contract

- Implement only this function; do not write `main`; these functions are called directly.
- Write only the first `cap` bytes of `dst`; bytes after `cap` stay as they were.
- The source string ends with `\0`; reading stops there and never goes past it.
