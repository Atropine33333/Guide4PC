Read three integers `a`, `b`, and `c`, then print their product.

**Input**: one line with three integers, each with absolute value at most `10^9`. The product is guaranteed to lie in `[-9 × 10^18, 9 × 10^18]`.

**Output**: one line with the integer `a × b × c`.

### Example

Input:

```text
2 3 4
```

Output:

```text
24
```

### Constraints

- Multiplying two values near `10^9` already exceeds 32-bit `int`; do not multiply as `int` and convert afterward.
- The product may be negative or zero.
- Use a signed 64-bit integer for the intermediate result.
