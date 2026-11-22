LX-24 LABEL PRINTER — USER MANUAL

1. Purpose
The machine joins a goods name and a goods code into one label, or writes a quotation inside square brackets.

2. Label format
A goods label is `name#code`, written out in one run; a quotation label is `[text]`.

3. Capacity
Every label is written into a buffer of `cap` bytes, and the last byte is reserved for the terminating `'\0'`. At most `cap - 1` visible characters.

4. When it does not fit
Cut at the tail: the characters that fit are written as they are, then `'\0'`. No warning, no line break, no padding.

5. Return value
The number of visible characters actually written, not counting `'\0'`. With `cap` equal to 1 nothing is written and the function returns 0.

6. Caution
The supplied sample program never looks at the capacity: it copies with `strcpy` and `strcat` all the way, so a long name writes past the buffer and tramples the neighbouring memory. Correct it according to this manual.

**In plain words**: implement two functions. `label_make` joins `name`, `#` and `code` into `out`; `label_quote` writes `text` wrapped in square brackets into `out`. Both write at most `cap - 1` characters, terminate with `'\0'`, and return the number of characters written. The versions in `starter.c` ignore `cap`; fix them.

**Interface**:

```c
#define LABEL_CAP 24

int label_make(char *out, int cap, const char *name, const char *code);
int label_quote(char *out, int cap, const char *text);
```

### Example

`label_make(buf, 24, "A7", "B2")` produces `A7#B2` and returns `5`.

`label_quote(buf, 24, "ok")` produces `[ok]` and returns `4`.

### Contract

- `cap` is the capacity of `out`, including the terminating `'\0'`, and is at least `1`.
- Truncation happens only at the tail; earlier characters are kept as they are.
- The return value equals the number of visible characters written.
- These functions are called directly; do not write `main`.
- Joining and cutting must both respect `cap`; no dynamic allocation.
