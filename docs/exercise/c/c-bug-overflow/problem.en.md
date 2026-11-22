(A storyteller's block is struck.)

Now hear of Hejian Prefecture, and of a bookkeeper named Zhou, known to all as Zhou the Abacus. One day the master brought in a stack of slips and asked for the takings to be tallied.

Zhou clicked his beads and added one slip after another: "ten strings, a hundred, a thousand..." At the end he looked up and called out a number.

The master nearly jumped: "Negative? Does this stack of mine owe you money instead?"

Zhou looked down. His old abacus only knew nine digits; past two billion it rolled over, and the account could not be trusted. With a new abacus the whole hundred thousand strings of cash came out right.

**In plain words**: the program in `starter.c` reads `n` amounts and adds them up. It accumulates in an `int`, so past two thousand million the total rolls over into a negative number and the output is wrong. Move the accumulator and the printing to `long long`.

**Input**: the first line holds an integer `n` (`1 ≤ n ≤ 1000`); the second line holds `n` integers, each between `0` and `10^9`, separated by single spaces.

**Output**: one line, the total of the `n` amounts.

### Example

Input:

```text
10
1000000000 1000000000 1000000000 1000000000 1000000000 1000000000 1000000000 1000000000 1000000000 1000000000
```

Output:

```text
10000000000
```

### Contract

- The total approaches `10^12`; keep it in a `long long`.
- Change only the broken spot; keep the input and output format as it is.
- Print one line, ending with a newline.
