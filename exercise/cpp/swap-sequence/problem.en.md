TAROT — THE SHUFFLE

The querent takes a seat; the spread is already laid out. The reader says: "The cards are exactly these numbers, no more and no fewer. All you do is take the two positions written on each card and exchange the cards that lie there."

The querent draws "one" and "five", and the two change places; then "two" and "four", which also change places. The reader does not look at the faces, only at the positions, and says: "The way of the shuffle is exchange in place; a copy elsewhere does not count."

"And if the two positions point to the same card?"

"Then it stays where it is, and the count of cards is unchanged."

"And if no card is drawn at all?"

"The spread remains as it was, not one card moved."

The querent nods and pushes the cards back: "Then read me the final spread."

**In plain words**: read `n`, then `n` integers, then `m` pairs of positions, swap the two elements at each pair, and print the final sequence. Positions are numbered from 1, `m` may be 0, and `i` and `j` may be equal. The starter already handles input, the calls, and output; only the swap function is missing, and its parameters must be references: with values, the function swaps copies and the caller's array stays as it was.

### Input

- The first line contains an integer `n` (`1 ≤ n ≤ 200`).
- The second line contains `n` integers with absolute value at most `10^9`.
- The third line contains an integer `m` (`0 ≤ m ≤ 200`).
- The next `m` lines each contain two integers `i` and `j` (`1 ≤ i, j ≤ n`), meaning the `i`-th and `j`-th elements should be swapped.

### Output

One line with the final `n` integers, separated by single spaces.

### Example

Input:

```text
5
1 2 3 4 5
2
1 5
2 4
```

Output:

```text
5 4 3 2 1
```

### Hint

The starter already implements input, the calls, and output; only the swap function is missing. If its parameters are passed by value, the function swaps copies and the caller's array stays unchanged.
