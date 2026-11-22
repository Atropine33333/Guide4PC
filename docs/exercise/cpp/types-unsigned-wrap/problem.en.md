Notes on Unsigned Subtraction

1. `a` and `b` are both unsigned 64-bit integers, taking values from `0` to `18446744073709551615`.
2. Print `a - b` and `b - a`, two integers on one line, separated by a single space.
3. Subtraction is performed modulo `2^64`. When the mathematical result is negative, add `2^64`.
4. Example: `a = 3`, `b = 5`. Then `a - b` is `18446744073709551614` and `b - a` is `2`.
5. Rule 3 applies to both outputs at once. There is no "negative number" entry in the output.
6. If you keep the intermediate in a signed integer, rule 2 is void for you: you will get a negative number, or worse.
7. Both results are printed in decimal, with no padding and no prefix.

**In plain words**: read two unsigned 64-bit integers `a` and `b` and print `a - b` and `b - a` modulo `2^64`, separated by a single space. Subtraction never yields a negative here; it wraps.

**Input**: one line with two integers `a` and `b`, each from `0` to `18446744073709551615`.

**Output**: one line with two integers separated by a single space: first `a - b`, then `b - a`.

### Example

Input:

```text
3 5
```

Output:

```text
18446744073709551614 2
```

In unsigned 64-bit arithmetic, `3 - 5` equals `2^64 - 2`.

### Constraints

- Print both results in decimal, with no padding or prefix.
- Do not store intermediate results in a signed integer: that gives a negative value or undefined behaviour.
- Both inputs are unsigned; read them as `unsigned long long`.
