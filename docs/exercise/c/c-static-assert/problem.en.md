《Rules at Compile Time》

Rule one: on this machine `int` takes four bytes. This rule must hold before the program starts running.
Rule two: when `sizeof(int)` is not `4`, the compiler must not let the code through.
Rule three: the rule is written at file scope with `_Static_assert`; the line printed at run time only reports the result to the reader.
Rule four: read an integer `n`, report `n`, then report the size of `int` and the result of the check.
Rule five: do not move the rule inside a function, and do not wait until the program runs to ask whether it holds.

**In plain words**: Put `_Static_assert(sizeof(int) == 4, "int must be 4 bytes")` at file scope; read a decimal integer `n` and print one line `n=<n> int=4 check=ok`.

**Input**: one line with a decimal integer `n`, where `-2147483648 ≤ n ≤ 2147483647`.

**Output**: one line such as `n=42 int=4 check=ok`. The value after `int=` is `sizeof(int)` in decimal, which is `4` here; the value after `check=` is `ok`.

### Example

Input:

```text
42
```

Output:

```text
n=42 int=4 check=ok
```

### Contract

- Put `_Static_assert` at file scope, outside `main`; its second argument is the message shown when the check fails.
- After including `<assert.h>` you may write `static_assert` instead; `_Static_assert` is enough here.
- Convert `sizeof(int)` to `int` before printing it with `%d`.
- The input is guaranteed to fit in `int`, so no overflow occurs.
- No trailing spaces at the end of the line.
