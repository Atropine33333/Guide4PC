Now hear ye the tale of the company's one meeting room, and the long queue of departments that would fain possess it.

Each meeting hath a start and an end; when one is dismissed, the next may enter. End to end—one ends as the next begins—is no quarrel. A meeting may begin and end in the same hour, taking no time, yet it counteth as one.

How many meetings, at most, can be held in a day?

**In plain words**: given `n` intervals `[s, e]`, select the largest set of mutually non-overlapping intervals and output its size.

### Input

The first line contains an integer `n`. The next `n` lines each contain two integers `s` and `e`, meaning a meeting starts at `s` and ends at `e`. `s == e` is allowed: such a meeting takes no time but still counts as scheduled.

### Output

One integer: the maximum number of meetings that can be scheduled.

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

They touch end to end, so all three fit.

### Constraints

- `1 <= n <= 100000`
- `0 <= s <= e <= 1000000000`
