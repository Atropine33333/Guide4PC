Read an integer `n` and factor it into primes, printed from smallest to largest.

**Input**: one integer `n` (`2 ≤ n ≤ 10^12`).

**Output**: one line of terms shaped `p^e`, separated by single spaces, where `p` is prime and `e` is its exponent; write `p^1` when the exponent is 1.

### Example

Input:

```text
12
```

Output:

```text
2^2 3^1
```

### Constraints

- Prime factors are ordered from smallest to largest.
- Each prime appears only once; write the exponent in decimal without leading zeros.
- When `n` itself is prime, print one term `n^1`.
- Trial division up to `sqrt(n)` is fast enough; remember to emit the remaining factor when it is greater than 1.
