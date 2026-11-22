Put two `int` values together and multiply: the result is first computed at `int` width, so any high bits that do not fit are lost. To keep every digit, invite `long long` to the table.

**In plain words**: read two integers and print their product. Compute the product as a 64-bit signed integer, so the intermediate result does not stop at `int` width.

**Input**: one line with two integers `a` and `b`, where `-10^9 ≤ a ≤ 10^9` and `-10^9 ≤ b ≤ 10^9`.

**Output**: one line with a single integer, the exact value of `a * b`.

### Example

Input:

```text
100000 100000
```

Output:

```text
10000000000
```

`10^10` lies beyond the range of a 32-bit `int`, so the multiplication must be done at 64-bit width.

### Contract

- Convert `a` to `long long` before multiplying: `(long long)a * b`.
- The product reaches at most `10^18`, which `long long` can hold.
- Print with `%lld`.
- No trailing spaces at the end of the line.
