THE CODE OF C++ OPERATOR PRECEDENCE

Article 1 (Purpose). This Code sets out the binding order of operators where parentheses are omitted.

Article 2 (Multiplication). In `a + b * c`, multiplication binds before addition; it is equivalent to `a + (b * c)`.

Article 3 (Parentheses). In `(a + b) * c`, the addition inside the parentheses comes first.

Article 4 (Shift and Subtraction). In `a << b - 1`, subtraction binds before the left shift; it is equivalent to `a << (b - 1)`.

Article 5 (Bitwise AND and XOR). In `a & b ^ c`, bitwise AND binds before XOR; it is equivalent to `(a & b) ^ c`.

Article 6 (Relational and Equality). In `a < b == c > a`, relational operators bind before equality; it is equivalent to `(a < b) == (c > a)`.

Article 7 (Unary Minus). In `-a * -b`, unary minus binds before multiplication; it is equivalent to `(-a) * (-b)`.

Article 8 (Subtraction Associativity). In `a - b - c`, subtraction is left-associative; it is equivalent to `(a - b) - c`.

Article 9 (Conditional Operator). In `a ? b : c`, the conditional operator is right-associative; this exercise contains only one conditional operator.

Article 10 (Supplementary). Matters not provided in this Code are governed by the C++ standard. No expression may be given parentheses of one's own, nor may the habits of another language be invoked.

**In plain words**: read `q a b c` and evaluate expression `q` under C++ precedence and associativity; do not add parentheses of your own. Every intermediate result fits in `int`, so nothing overflows.

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
