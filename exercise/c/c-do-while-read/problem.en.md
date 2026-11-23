THE CODE OF READING

Article 1 (First read). Whoever reads integers reads one first, and only then considers the rest. Before a single value is read, the loop has already run once.

Article 2 (Stopping). The value zero ends the reading at once; zero is not entered into the account.

Article 3 (Counting). Every nonzero value read is one value counted.

Article 4 (Summing). Every nonzero value read is added to the total. The total is kept as a long integer; a thousand values of a billion each still cannot overflow it.

Article 5 (Empty scroll). When the first value is zero, the count is zero and the sum is zero.

Article 6 (Verdict). When reading ends, report the count first and the total second, on one line, separated by a single space.

**In plain words**: keep reading integers until a `0` arrives; the `0` itself does not count. Print the number of nonzero values read and their total (count first, then total). The loop body runs at least once.

**Input**: one line of integers ending with `0`; at most `1000` values precede the `0`, each with absolute value at most `10^9`.

**Output**: one line with two integers — the count and the total — separated by a single space.

### Example

Input:

```text
3 5 -2 0
```

Output:

```text
3 6
```

### Contract

- Write the loop with `do`-`while`: read one value, then decide whether to stop, so the first value is always read.
- The input ends with `0`; the `0` is neither counted nor added.
- The total can exceed the range of `int`; keep it in a `long long`.
- No trailing spaces at the end of the line.
