The station's hull is ruled into byte-sized cells. An engineer seals two samples into the alignment bay: sample A is one character cell plus one integer cell; sample B adds a second character cell after the integer. The hull slips invisible spacers between members so that every `int` starts at a multiple of four. The probe reports only four readings: how many bytes A takes, how many B takes, where `i` begins in A, and where `d` begins in B. No readings, no hatch.

**In plain words**: define `struct A { char c; int i; }` and `struct B { char c; int i; char d; }`, then print `sizeof(struct A)`, `sizeof(struct B)`, `offsetof(struct A, i)`, and `offsetof(struct B, d)` on one line.

**Input**: none.

**Output**: one line with four integers separated by single spaces.

### Example

Output:

```text
8 12 4 8
```

### Contract

- `char` takes 1 byte, `int` takes 4 bytes, and every `int` starts at a multiple of 4.
- Gaps appear between `c` and `i`, and again after `d`; `sizeof` counts those gaps in the total.
- Use `offsetof` from `<stddef.h>` and print all four values with `%zu`.
- No trailing spaces at the end of the line.
