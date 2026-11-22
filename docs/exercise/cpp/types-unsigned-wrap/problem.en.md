Read two unsigned 64-bit integers `a` and `b` and print `a - b` and `b - a`. Subtraction wraps modulo `2^64`: a negative mathematical result is increased by `2^64`.

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
