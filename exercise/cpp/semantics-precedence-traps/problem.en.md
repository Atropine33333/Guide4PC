Read an expression number `q` and three positive integers `a`, `b`, and `c`, then evaluate expression `q` below. The expressions deliberately omit parentheses; follow C++ precedence and associativity exactly.

```text
1: a + b * c
2: (a + b) * c
3: a << b - 1
4: a & b ^ c
5: a < b == c > a
6: -a * -b
7: a - b - c
8: a ? b : c
```

### Input

One line with four integers `q a b c` such that `1 <= q <= 8` and `1 <= a, b, c <= 9`.

### Output

One line containing the value of expression `q`.

### Example

Input:

```text
4 2 3 4
```

Output:

```text
6
```

### Constraints

- Every intermediate result fits in `int`; no overflow occurs.
- In `a << b - 1`, the value `b - 1` is between 0 and 8, so the shift is well defined.
- Use only the standard library; do not print anything beyond the answer.
