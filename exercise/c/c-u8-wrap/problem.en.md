An eight-bit unsigned accumulator starts at `0`. Read `n` integers one after another and add each to the accumulator. The accumulator can only hold `0` to `255`: when a sum passes `255`, the high bits are dropped and counting restarts from `0` (that is, the sum is taken modulo `256`). After every addition, print the current accumulator value.

**Input**: the first line holds an integer `n` (`1` ≤ `n` ≤ `1000`). Then come `n` integers, each between `0` and `255`, separated by whitespace and possibly spread over one or more lines.

**Output**: `n` lines. Line `i` is the accumulator value after adding the `i`-th number, between `0` and `255`, in decimal with no leading zeros.

### Example

Input:

```text
3
200 100 56
```

Output:

```text
200
44
100
```

### Contract

- The accumulator variable must be a `uint8_t`, initialised to `0`.
- Each addition wraps modulo `256` when assigned back to the `uint8_t`; for instance `200 + 100 = 300` becomes `44`.
- Print each accumulator value on its own line, ending with a newline.
- Do not pad with leading zeros and do not add a sign.
