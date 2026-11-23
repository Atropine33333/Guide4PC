**PRODUCT MANUAL**

**Product**: Plate Reader
**Model**: `union` reuse
**Use**: read one four-byte plot two ways. Fill the plate as an integer, then read a fraction from the very same plot.

**Instructions**:

1. The plot has a single plate, four bytes wide. The plate carries an 8-digit hexadecimal number, upper or lower case, with no `0x` prefix.
2. Two households live on this plot. One reads integers, the other reads fractions. You fill in the plate as an integer, and the fraction household reports from the same plot.
3. Nobody may carry off a single byte of the ground. The plot never grows and never shrinks.
4. The fraction household reports 6 significant digits, dropping trailing zeros; very large or very small numbers switch to exponent form.
5. When the plate reads `3f800000`, that household reports `1`. Keep this one by heart.
6. Write nothing besides the report.

**In plain words**: read an 8-digit hexadecimal number, store it unchanged in the `unsigned` member of a `union`, then read it back from the `float` member of that same `union` and print it with `%.6g`.

**Input**: one line with an 8-digit hexadecimal number, either case, without the `0x` prefix.

**Output**: one line with the value of those 32 bits read as a `float`, printed with `%.6g`.

### Example

Input:

```text
40490fdb
```

Output:

```text
3.14159
```

The same four bytes of ground, filled in as an integer, read out as a fraction give `3.14159274`; `%.6g` keeps 6 significant digits, which is `3.14159`.

Input:

```text
3f800000
```

Output:

```text
1
```

### Contract

- The two members of the `union` share one piece of memory: writing one and reading the other reads back the very same bits.
- Inputs are always 8 hexadecimal digits, exactly 32 bits, ranging from `00000000` to `ffffffff`.
- Every plate given corresponds to a finite fraction, never to the encoding of infinity or NaN.
- No trailing spaces at the end of the line, and nothing beyond the answer.
