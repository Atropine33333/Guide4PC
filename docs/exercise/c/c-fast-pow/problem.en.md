Below Mount Shaoshi the old master pushes a cup of tea toward you.

"To drive `b` measures of inner force through `e` gates," he says, "pushing one palm at a time will empty you before the gates are done."

He splits his palm in two. "Watch closely. Halve the palm, and its force squares itself; halve the gates, and keep the odd one on account. An even count of gates needs only one squaring; an odd count puts one palm on the ledger first. Sixty gates take no more than a few dozen turns of the hand."

You follow his lead, and at the last gate the force lands on `m` flagstones. Whatever is left over is your answer.

**In plain words**: compute `b` raised to the power `e`, modulo `m`, i.e. `b^e mod m`.

**Input**: one line with three integers `b`, `e` and `m` (`0 ≤ b < m`, `0 ≤ e ≤ 10^18`, `1 ≤ m ≤ 10^9`).

**Output**: one integer, `b^e mod m`, in the range `0` to `m - 1`.

### Example

Input:

```text
2 10 1000
```

Output:

```text
24
```

### Contract

- For `e = 0` output `1 % m`: that is `0` when `m = 1`, and `1` otherwise.
- For `m = 1` output `0` for every exponent.
- `e` can reach `10^18`; multiplying one step at a time will time out. Split the exponent in binary and take the modulus after every multiplication — about sixty rounds.
- The largest intermediate product is about `(10^9)^2 = 10^18`, which fits in a `long long`; reduce modulo `m` immediately after each multiplication.
- No trailing spaces at the end of the line.
