SEATING BY SCORE

The reader fans a few cards, each bearing a name and a score.

"There is but one row of seats," she says. "The higher score sits nearer the front; among equal scores, the name that comes first in the dictionary sits first."

She hands you the cards: "Seat them by this rule and read them out, one to a line. Name first, score second."

**In plain words**: read `n` students, each with a name and a score; sort them with `qsort` by score from high to low, breaking ties by name from small to large; then print `name score` for each in that order.

**Input**:

- The first line holds an integer `n` (`1 <= n <= 100`).
- The next `n` lines each hold a name and an integer score, separated by a single space; a name contains only lowercase letters and is between `1` and `20` characters long; a score lies between `0` and `1000`.

**Output**: `n` lines, each holding a name and a score separated by a single space.

### Example

Input:

```text
4
dan 80
alice 90
cara 80
bob 70
```

Output:

```text
alice 90
cara 80
dan 80
bob 70
```

`alice` has the highest score and takes the first seat; `cara` and `dan` both have `80`, and `cara` comes first in the dictionary, so `cara` sits before `dan`; `bob` has the lowest score and sits last.

### Contract

- Sort with `qsort`; compare names with `strcmp` and scores as integers.
- A higher score comes first; among equal scores, the smaller name comes first.
- Names are distinct, so no name-score pair repeats.
- No trailing spaces at the end of the line.
