Read two DNA strands of equal length and report their Hamming distance and the reverse complement of the first strand.

The complement pairs are `A`-`T` and `C`-`G`. The reverse complement is obtained by reversing the strand and then replacing every character with its complement.

### Input

Two lines, each containing one DNA strand. Both strands have the same length and contain only uppercase `A`, `C`, `G`, and `T`.

### Output

- Line 1: the number of positions where the two strands differ.
- Line 2: the reverse complement of the first strand.

### Example

Input:

```text
AACG
TAGC
```

Output:

```text
3
CGTT
```

### Constraints

- Each strand has length between 1 and 100.
- The two strands always have equal length.
- Use only the standard library; do not print extra spaces or blank lines.
