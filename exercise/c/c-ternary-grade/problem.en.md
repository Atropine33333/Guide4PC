RATING BY TERNARY — USER MANUAL

1. Purpose. Fold a score out of one hundred into one of five grades: A, B, C, D, F.

2. Operation. Feed one integer from `0` to `100` into the input port and press Enter; the grade letter appears on the screen.

3. Scale. `90` and above is A; `80` to `89` is B; `70` to `79` is C; `60` to `69` is D; below `60` is F.

4. Inside. The unit holds a single nested conditional expression. It tests from the highest band downwards, and the first condition that holds is the answer.

5. Note. A boundary value belongs to the higher band: `90` is A, `89` is B, and so on.

**In plain words**: read a score from `0` to `100`, decide its band with a nested conditional expression, and print one uppercase letter.

**Input**: one line with a single integer `score` (`0 ≤ score ≤ 100`).

**Output**: one line with one uppercase letter: `A`, `B`, `C`, `D`, or `F`.

### Example

Input:

```text
89
```

Output:

```text
B
```

### Contract

- Decide with one nested conditional expression, with the tests ordered from high to low.
- Each band includes its lower bound and excludes the lower bound of the band above it.
- Print only the letter and a newline; no trailing spaces.
