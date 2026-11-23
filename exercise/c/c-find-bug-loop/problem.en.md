The program in `starter.c` should compute `1 + 2 + ... + n`. Read `n` and fix the wrong output.

As it stands, for every `n` it adds exactly one term too few: the result is smaller than the correct sum by `n`.

**Input**: one line with an integer `n` (`1 <= n <= 1000000`).

**Output**: one line, the sum from `1` to `n`.

### Example

Input:

```text
10
```

Output:

```text
55
```

### Contract

- The largest sum is `500000500000`, beyond 32-bit range; store it in a `long long`.
- Change only the broken part; keep the input and output format as it is.
- Print one line, ending with a newline.
