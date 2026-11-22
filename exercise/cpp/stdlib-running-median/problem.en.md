Huizi said to Zhuangzi: "Numbers arrive, one after another, again and again, and there is no end to them. With each new number, I wish to know the lower median. How shall it be found?"

Zhuangzi said: "Do you not see the numbers? A rod one foot long, halved each day, will not be exhausted in ten thousand ages. So it is with numbers: divide them in two, the smaller on the left and the larger on the right. The top of the left heap is the lower median."

Huizi said: "Then how shall the left and right be kept in balance?"

Zhuangzi said: "The left heap shall hold no fewer than the right, and at most one more. Each new number goes first to the left; if the left top is greater than the right top, exchange them. If the left grows too large, move one to the right; if the right grows too large, move one to the left. Thus the middle is ever at the left top, ready to be taken."

Huizi said: "What of repeated numbers?"

Zhuangzi said: "Each copy counts as one card; do not merge them. Sorting and then picking also works, but that sorts anew for every number, an immense waste of time, and no good plan."

**In plain words**: Read `n` integers one at a time. After each insertion, print the lower median of all values seen so far: after sorting them ascending, the value at index `(m - 1) / 2`, where `m` is the current count and indices start at 0. Print one line of `n` integers separated by single spaces. With an even count take the lower middle value, without averaging; duplicates count separately. Sorting from scratch each time is too slow, so maintain the median with two heaps or a balanced structure.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 200000`).
- The second line contains `n` integers, each between `-10^9` and `10^9`.

**Output**: one line with `n` integers separated by single spaces; the `i`-th is the lower median after inserting the first `i` values.

### Example

Input:

```text
4
10 20 30 40
```

Output:

```text
10 10 20 20
```

After 10 the set is `[10]`, median 10; after 20 it is `[10, 20]`, lower median 10; after 30 it is `[10, 20, 30]`, middle value 20; after 40 it is `[10, 20, 30, 40]`, lower median 20.

### Constraints

- With an even count, take the lower of the two middle values, without averaging.
- Duplicate values count separately.
- Sorting after every insertion is too slow; maintain the median with two heaps or a balanced structure.
