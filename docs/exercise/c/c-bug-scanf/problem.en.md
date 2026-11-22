The program in `starter.c` reads two integers and should print their sum. The second integer is never read: `scanf` receives the value of `b` instead of its address, so the input goes somewhere else. Fix the argument.

**Input**: one line with two integers `a` and `b`, `-10^9 ≤ a, b ≤ 10^9`, separated by a single space.

**Output**: one line, the value of `a + b`.

### Example

Input:

```text
12 30
```

Output:

```text
42
```

### Contract

- Both numbers fit in `int`, and so does their sum; nothing overflows.
- Change only the broken spot; keep the input and output format as it is.
- Print one line, ending with a newline.
