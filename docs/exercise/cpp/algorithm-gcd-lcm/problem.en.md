Read several pairs of integers and output the greatest common divisor and least common multiple of each pair.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 1000`).
- The next `n` lines each contain two integers `a` and `b` (`0 ≤ a, b ≤ 10^9`).

**Output**: `n` lines with two integers separated by a single space: the GCD and the LCM of `a` and `b`.

### Example

Input:

```text
2
12 18
7 5
```

Output:

```text
6 36
1 35
```

### Constraints

- `gcd(0, 0) = 0`.
- If either value is 0, the LCM is 0.
- The LCM may be close to `10^18`; store it in a 64-bit integer.
- When computing the LCM, divide before multiplying to avoid overflowing an intermediate result.
