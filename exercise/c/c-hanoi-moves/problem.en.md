A: Three pegs, and `n` disks stacked on the first, smallest on top.

B: All of them must move to the third peg.

A: One disk at a time, and a larger disk may never rest on a smaller one. What is the fewest moves?

B: Move the top `n - 1` disks to the middle peg, move the largest onto the third peg, then move those `n - 1` disks on top of it.

A: So every step doubles the work and adds one more?

B: Exactly: `T(n) = 2T(n - 1) + 1`, and with one disk it is a single move. Unroll it and you get `2^n - 1`.

A: Well then — name a number and let the listener work it out.

**In plain words**: read `n` (`1 ≤ n ≤ 30`) and print the fewest moves needed to carry `n` disks to the third peg, namely `2^n - 1`.

**Input**: one line with an integer `n` (`1 ≤ n ≤ 30`).

**Output**: one integer — the fewest moves.

### Example

Input:

```text
3
```

Output:

```text
7
```

Three disks: small to peg three, medium to peg two, small to peg two, large to peg three, small to peg one, medium to peg three, small to peg three — seven moves.

### Contract

- `2^30 - 1 = 1073741823` fits in `int`; a `long long` is the safer choice.
- Print only the number of moves, not the sequence of moves.
- No trailing spaces at the end of the line.
