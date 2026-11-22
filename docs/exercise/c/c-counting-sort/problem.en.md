*Rules for the Night Roll Call*

Rule 1. Numbers on the roster fall between `0` and `100`, no more and no less.

Rule 2. When names are called, the smaller number answers first.

Rule 3. A number may belong to more than one person. Whoever joined the line first answers first, and that order may not be reversed.

Rule 4. You may not drag two people together to compare them. Count heads only.

Rule 5. When the calling ends, copy the numbers onto one line in the order they answered, with one space between numbers.

Rule 6. If the roster holds no names at all, hand back a blank sheet.

Rule 7. If you hear someone behind you reciting this rule, start the roll call over at once.

**In plain words**: read `n` and `n` integers between `0` and `100`, then perform a stable counting sort — smaller values first, equal values in their original input order — and print the sorted numbers; for `n = 0`, print an empty line.

**Input**: the first line holds an integer `n` (`0 ≤ n ≤ 1000`); the second line holds `n` integers, each between `0` and `100`, separated by single spaces; when `n` is `0` the second line may be empty.

**Output**: one line with the `n` sorted integers separated by single spaces; when `n` is `0`, one empty line.

### Example

Input:

```text
6
3 1 2 3 1 2
```

Output:

```text
1 1 2 2 3 3
```

### Contract

- Stable: elements with equal values keep their original input order.
- Count how often each value occurs, then emit values in order; do not compare pairs.
- Values range from `0` to `100`, and `n` is at most `1000`.
- For `n = 0`, print an empty line.
- No trailing spaces at the end of the line.
