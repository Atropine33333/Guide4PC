This exercise reads no stdin. Evaluate an arithmetic expression containing integers, `+`, `-`, `*`, parentheses, and unary minus.

```cpp
#include <string>

int eval_expression(const std::string& text);
```

Grammar (`expression` is the entry point):

```text
expression := term (('+' | '-') term)*
term       := factor ('*' factor)*
factor     := number | '(' expression ')' | '-' factor
number     := one or more decimal digits
```

Rules:

- Operators are left-associative: `10 - 3 - 2` is `5`, and `2 * 3 * 4` is `24`.
- Unary minus may appear in any factor position and may nest: `-5`, `2 * -3`, `--4`, `-(-3)`, `-(1 + 2)`.
- Spaces may appear between any two tokens (including before a number) and do not affect the result.
- The input is guaranteed to be a valid expression and its result fits in `int`.
- Do not write `main`.

The starter's parser handles binary `+ - *` and parentheses but mishandles unary minus; fix it.

### Example

`2 * -3 + (4 - 1)` evaluates to `-3`.

### Hint

Look at the `factor` parsing: when a `'-'` is found, it should be consumed and the following `factor` negated, not just return `0`.
