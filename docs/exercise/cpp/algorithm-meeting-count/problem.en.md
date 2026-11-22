There is a single meeting room and `n` meeting requests, each given by a start and an end time. Two meetings can both be scheduled if the earlier one ends no later than the later one starts. Find the maximum number of meetings that can be scheduled.

### Input

The first line holds one integer `n`. The next `n` lines each hold two integers `s` and `e`, a meeting running from `s` to `e`. `s == e` is allowed: such a meeting occupies no time but still counts as scheduled.

### Output

One integer: the maximum number of meetings.

### Example

```
3
1 2
2 3
3 4
```

Output:

```
3
```

The three meetings touch end to start, so all can run.

### Constraints

- `1 <= n <= 100000`
- `0 <= s <= e <= 1000000000`
