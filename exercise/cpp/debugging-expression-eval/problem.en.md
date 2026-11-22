Zhuangzi and Huizi were strolling on the bridge over the Hao.

Huizi said: "Tell me about reckoning."

Zhuangzi said: "An expression has a grammar. First, a number: one or more decimal digits. Second, a factor: a number, or a parenthesised expression, or a minus sign standing before a factor. Third, a term: factors multiplied together. Fourth, an expression: terms added and subtracted. Inside parentheses, an expression begins again."

Huizi said: "When a minus sign leads, may the reckoner simply take the value as zero?"

Zhuangzi said: "He may not. A foot-long rod, halved each day, is never exhausted in ten thousand ages. Recursion cuts and rejoins; while the cut is unfinished, the expression is unfinished. When `-` stands before a factor, consume it and negate what follows. Two minuses make the value positive again, as a rod cut twice grows back."

Huizi said: "And addition against multiplication — which comes first?"

Zhuangzi said: "Multiplication comes before addition; addition and subtraction take the rear. Among equals, work from left to right and do not transpose them. Spaces may sit between any two tokens and do no harm."

Huizi said: "Good. Write the grammar below, as a standard for later reckoners."

**In plain words**: implement `eval_expression(text)` for expressions containing integers, `+`, `-`, `*`, parentheses and unary minus, parsed top-down: `expression` splits into `term`, and `term` into `factor`. The starter returns `0` whenever it meets a unary minus; change it to consume the `-` and negate the following factor. Do not write `main`.

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

### Example

`2 * -3 + (4 - 1)` evaluates to `-3`.

### Hint

Look at the `factor` parsing: when a `'-'` is found, it should be consumed and the following `factor` negated, not just return `0`.
