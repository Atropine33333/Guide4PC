Welcome to the Summing Room.

Every rule here is true. The room does not lie. It only replaces the word "integer" in your sentence with something else.

Rule One: this room's `int` is thirty-two bits wide and stops at 2147483647. The sum in your hands may reach 2 × 10^14, and it does not fit.

Rule Two: someone once reported a sum of `-1294967296`. That is 3000000000 wearing an old coat, wrapped around from the 2147483647 end. Treat it as that very large number.

Rule Three: Rule Two holds in this room only. Move the ledger to sixty-four bits and the coat comes off; move it back to `int` midway and the coat returns.

Rule Four: the ledger keeps one line. The first number is written down, then the second; the order is not yours to change.

Rule Five: the door opens from the outside. Read the input as given, write the output as given, and touch only the ledger's type.

If you notice that `-1294967296` is starting to feel familiar, go back to the first sentence and read it again.

**In plain words**: read `n` integers and print their sum. The sum may reach 2 × 10^14 in absolute value, so accumulate in `long long`; the starter stores it in an `int` and wraps around into a negative number. Fix that one spot and keep the input and output format.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 200000`).
- The second line contains `n` integers, each with absolute value at most `10^9`.

The total sum `S` is guaranteed to satisfy `-2 × 10^14 ≤ S ≤ 2 × 10^14`.

**Output**: one line with the sum.

### Example

Input:

```text
3
1000000000 1000000000 1000000000
```

Correct output:

```text
3000000000
```

The starter prints a negative number instead. Change only what is necessary; keep the input and output format.
