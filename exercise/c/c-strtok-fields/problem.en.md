**Product Manual**

**Name**: Field Splitter
**Model**: `split_fields`
**Use**: a single line of comma-separated text

**1. Purpose**

Cut one line of text at its commas, trim the spaces at both ends of each piece, drop empty pieces, and place the remaining pieces into slots prepared by the caller.

**2. Operation**

The caller prepares a row of slots `out`, each `FIELD_LEN` bytes wide, and offers at most `max` of them. Hand the whole line `line` to `split_fields`; the function returns the number of pieces actually stored.

**3. Example**

`"  alpha ,, beta  , gamma "` yields three fields: `alpha`, `beta`, `gamma`.

`"one,two,three,four,five"` with `max = 3` stores only `one`, `two`, `three` and returns `3`.

**In plain words**: split a line at commas, trim spaces around each piece, skip empty pieces, write the pieces into `out` in order and return how many there are; when there are more usable pieces than `max`, write only the first `max` and return `max`.

**Interface**:

```c
#define FIELD_MAX 32
#define FIELD_LEN 64

int split_fields(const char *line, char out[][FIELD_LEN], int max);
```

### Contract

- The only separator is the comma `,`. Each piece loses only leading and trailing ASCII spaces `' '`; spaces inside a piece are kept.
- A piece that is empty after trimming is skipped: it is not written to `out` and does not count toward the return value.
- Kept fields go into `out[0]`, `out[1]`, ... in their original order, each terminated by `'\0'`.
- A field longer than `FIELD_LEN - 1` (that is, 63) characters keeps only its first 63 characters and a terminating `'\0'`; it still counts as one field.
- When there are more usable fields than `max`, write only the first `max`, leave later slots untouched, and return `max`.
- When `max` is not greater than `0`, return `0` and do not touch `out`; `max` is never greater than `FIELD_MAX`.
- An empty line, or a line holding only commas and spaces, returns `0`.
- Do not write `main`; no dynamic allocation is needed.
