The camp strikes at midnight, and the clerk is handed one task: copy `n` characters of a document.

The pass is checked first: if the document never arrived (a null `s`), or the count is a negative number, the task fails and the troops withdraw at once.

Even a finished task needs a way back: if supplies run out halfway (memory runs short), the troops withdraw the same way rather than leave half a camp in the open field.

There is only one camp gate: every retreat follows the same `goto cleanup`, and there the gear is released, the accounts are settled, and nothing is left behind.

**In plain words**: implement `dup_n` — return `NULL` when `s` is null or `n < 0`; otherwise allocate `n + 1` bytes, copy at most `n` characters, stop at the end of the source, append `'\0'`, and return the new block.

**Interface**:

```c
char *dup_n(const char *s, int n);
```

- `s` is null, or `n` is negative: return `NULL` and allocate nothing.
- Allocation fails: return `NULL`.
- Success: return a new NUL-terminated string of at most `n` characters, taken from the front of `s`.

### Example

With `s` equal to `"hello"`: `n = 3` gives `"hel"`, `n = 5` gives `"hello"`, and `n = 8` still gives `"hello"`, because the copy stops at the end of the source.

### Contract

- Do not read past the source's `'\0'`, and do not modify the source.
- The caller releases the returned block with `free`.
- Every failure path joins the same cleanup before returning `NULL`.
- These functions are called directly; implement only `dup_n`, with no `main`.
