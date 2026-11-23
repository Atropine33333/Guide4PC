Implement `dup_string`: copy a NUL-terminated string with `malloc` and return the address of the new string.

**Interface**:

```c
char *dup_string(const char *s);
```

### Example

| Call | Result |
| --- | --- |
| `dup_string("hello")` | `"hello"` in a freshly allocated block |
| `dup_string("")` | an empty string in a freshly allocated block, first byte `'\0'` |

### Contract

- `s` is NUL-terminated; when its length is `0`, still return a block that can be passed to `free`—never `NULL`.
- The caller owns the result and frees it; do not return `s` itself.
- After the copy, changing what `s` points to must not affect the copy.
- Return `NULL` when memory runs out.
- Do not write `main`.
