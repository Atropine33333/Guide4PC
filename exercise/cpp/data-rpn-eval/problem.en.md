(O my dear friend, do sit down. On the table lies a sheet of paper with a string of tokens separated by spaces.)

"Good heavens," the gentleman said, pushing the sheet across, "look at this, will you. Two numbers first, the sign after — as though someone had said the sentence backwards."

I had a look. "You are describing reverse Polish notation, also called postfix. Each token is either an integer, minus sign and all, or one of `+`, `-`, `*`, `/`. The expression is guaranteed valid, and the divisor is never zero — of that you may be quite sure."

"Then I leave it to you, my dear fellow," he said, spreading his hands. "All this stack of paper, and I want only one number."

"A stack will do it," I said. "A number goes on top; an operator takes two down, the one pushed first counting as the left, and puts the result back. Whatever remains on top at the end is your answer. Division, mind you, is integer division, truncated toward zero — it will not embarrass you."

**In plain words**: read one line of space-separated reverse Polish (postfix) tokens and print the value of the expression. A token is either an integer (possibly negative) or one of `+`, `-`, `*`, `/`; the expression is valid and the divisor is never zero. Use a stack: push numbers, and on an operator pop two operands, treat the one pushed first as the left operand, compute, and push the result back; the final top is the answer. Division truncates toward zero, and intermediate and final results fit in a signed 64-bit integer. Use a standard-library stack or array; do not write a recursive-descent parser.

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
