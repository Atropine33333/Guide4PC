A row of integers stands in line. Report two things about them: their total, and the largest one. The numbers may all be negative, and the largest value may appear more than once; report it once.

**In plain words**: read an integer `n`, then read `n` integers, and print their sum and their maximum, separated by a single space.

**Input**: the first line holds an integer `n` (`1 ≤ n ≤ 1000`); the second line holds `n` integers, each with `-10^6 ≤ a[i] ≤ 10^6`.

**Output**: one line with two integers, the sum first and the maximum second, separated by a single space.

### Example

Input:

```text
5
3 -1 7 7 2
```

Output:

```text
18 7
```

The five numbers add up to `18`, and the largest is `7` (it appears twice; report it once).

### Contract

- `n ≥ 1`, so the array is never empty.
- The sum may exceed the range of `int`; store it in a wide enough integer type.
- Walk from the first element to the last, and do not step past either end.
- Do not print a trailing space at the end of the line.
