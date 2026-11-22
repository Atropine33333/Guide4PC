Read several records and print them sorted by the given rules.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 100`).
- The next `n` lines each contain a name and an integer score, separated by one space. Names consist of lowercase letters and have length 1–10; scores are in `-10000`–`10000`.

**Output**: `n` lines in the following order, each formatted as `name score`:

1. Higher scores first.
2. For equal scores, lexicographically smaller names first.

### Example

Input:

```text
3
alice 10
bob 30
cara 20
```

Output:

```text
bob 30
cara 20
alice 10
```

### Constraints

- The comparator must be a strict weak ordering: when scores tie, compare names only.
- Names are unique.
- Do not write a bubble sort; use `std::sort`.
