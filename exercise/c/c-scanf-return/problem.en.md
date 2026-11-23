Someone brings in the harvest, basket by basket. A basket may hold two pieces, or one, or none at all, and all he wants is the tally.

**In plain words**: write `read_pair`, which reads up to two integers out of the string `s` and returns how many it actually read. The values go into `*a` and `*b`; a pointer that was not read keeps whatever it held.

**Interface**:

```c
int read_pair(const char *s, int *a, int *b);
```

- Return `0`, `1` or `2`: the number of integers successfully read.
- The two integers are separated by whitespace, and extra whitespace before or after them is skipped.
- Only values that were actually read are written to `*a` and `*b`; the one that was not read stays untouched.
- When `s` is `NULL`, return `0` and leave both locations alone.

### Example

`read_pair("12 34", &a, &b)` returns `2`; `a` becomes `12` and `b` becomes `34`.

`read_pair("42", &a, &b)` returns `1`; `a` becomes `42` and `b` keeps its old value.

`read_pair("apple", &a, &b)` returns `0` and neither location changes.

### Contract

- Do the reading with `sscanf`; when nothing at all is read it may return a negative value, which must be normalised to `0`.
- Implement this one function only; do not write `main`.
- The function is called directly, and compiled code checks the return value and the written values.
