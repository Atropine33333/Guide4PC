Given a sequence of integers, find the element that occurs strictly more than half the time; if no such element exists, print `none`.

"Strictly more than half" means a count `> n / 2` (rounded down). For a sequence of length `4`, an element must appear at least `3` times; for length `5`, at least `3` times as well.

**Input**:

- The first line holds one integer `n` (`1 <= n <= 200000`).
- The second line holds `n` integers separated by single spaces, each within `[-10^9, 10^9]`.

**Output**:

- One line: the majority value, or `none`.

### Example

Input:

```text
5
2 2 1 2 3
```

Output:

```text
2
```

### Constraints

- The sequence has at most `200000` elements; a linear-time, constant-space algorithm is recommended.
- Print lowercase `none` when there is no majority element.
- An element occurring exactly `n / 2` times is not a majority element.
