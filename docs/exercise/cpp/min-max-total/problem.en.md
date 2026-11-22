Shuffle. Draw.

The querent lays out `n` cards, each face an integer, some positive and some not. The reader ignores the suits and looks at three things only: the lowest card, the highest card, and the sum of all the faces.

"The faces are just these numbers," she says, pushing the cards back. "The order they came in does not matter, and neither does how many there are. Ask me for fortune and I have no answer; ask me for the extremes and the total and one sentence settles it."

**In plain words**: read `n` integers, find the minimum, the maximum and the total, and print those three in that order on one line, separated by single spaces.

**Input**: the first line holds one integer `n` (`1 ≤ n ≤ 1000`). The second line holds `n` integers, each with absolute value at most `10^6`.

**Output**: one line with three integers — minimum, maximum, sum — separated by single spaces.

### Example

Input:

```text
5
3 -1 4 1 5
```

Output:

```text
-1 5 12
```

### Constraints

- The total may exceed the 32-bit `int` range; choose a suitable type for it.
- No extra trailing space at the end of the output line.
