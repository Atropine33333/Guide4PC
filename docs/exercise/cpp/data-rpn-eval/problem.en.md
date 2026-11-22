Read a space-separated reverse Polish (postfix) expression and evaluate it.

**Input**: one line of tokens separated by spaces. Each token is either an integer (possibly negative) or one of the operators `+`, `-`, `*`, `/`. The expression is valid and divisors are never zero.

**Output**: one line with the integer value of the expression.

### Example

Input:

```text
7 2 3 * -
```

Output:

```text
1
```

First `2 * 3 = 6`, then `7 - 6 = 1`.

### Constraints

- Division is integer division truncated toward zero: `-10 / 3 = -3`, `7 / -2 = -3`.
- Intermediate and final results fit in a 64-bit signed integer.
- Every operator takes two operands; the value pushed first is the left operand.
- Use a stack or an array from the standard library; do not write a recursive-descent parser.
