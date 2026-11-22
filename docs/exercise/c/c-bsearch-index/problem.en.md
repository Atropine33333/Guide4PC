"FIND IT IN LINE" — A STORYTELLER'S TALE

Now hear this. One day a thousand men were mustered on the drill ground, standing in a row from shortest to tallest, and no one was allowed to break the line. The order came down: "Find one number — if it stands in the row, report its place; if it is absent, answer `-1`."

Asking each man in turn would take a thousand questions. But the row is already in order, so the old clerk taught a trick: ask the man in the middle first; if his number is greater than the one sought, search the left half; if smaller, search the right half. Each question halves the crowd, and a few questions settle the matter. The trick is called `bsearch`.

**In plain words**: read a strictly increasing array of integers and a `key`, look the key up with `bsearch`, and print its index (starting at `0`); print `-1` when it is absent.

**Input**:

- The first line holds an integer `n` (`1 <= n <= 1000`).
- The second line holds `n` integers in strictly increasing order, each within the `int` range.
- The third line holds one integer, the `key`.

**Output**: one integer — the index of the `key` when found, or `-1` when it is absent.

### Example

Input:

```text
5
1 3 5 7 9
5
```

Output:

```text
2
```

`5` stands third in the row, at index `2`.

### Contract

- The array is strictly increasing, so each value appears at most once and the index is unique.
- Call `bsearch`, with a comparison function that compares `int` values.
- Indices start at `0`.
- No trailing spaces at the end of the line.
