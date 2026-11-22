(A small stage. A folding fan, two wooden clappers.)

A: Tonight I'll tell you about a knapsack. There are `n` items, and the pack holds `W` pounds. Each item has a weight and a value, and each can be taken at most once. How do we take the most value?

B: Easy! Sort by value per pound, highest first, and skip whatever no longer fits.

A: Sounds reasonable.

B: Sure. Look at these three: weight 2 value 3, weight 3 value 5, weight 4 value 6; the pack holds 6 pounds. Value per pound: 1.5, 1.67, 1.5.

A: So take the second one first.

B: Three pounds left; take the first. The pack is full, value 8.

A: Eight.

B: But there is a better haul: the first and the third. Weight 6, value 9.

A: (a pause) Your "best per pound" got eight.

B: So greedy stumbles here. Consider each item, take it or leave it, and record the best for "the first i items at capacity j".

A: That's dynamic programming?

B: Right. `dp[j]` is the best value at capacity `j`; update backwards so each item is used once. The answer may exceed 32 bits, so use 64-bit integers.

A: Good, let me read out the problem.

**In plain words**: 0/1 knapsack: each item is chosen at most once; maximize the total value without exceeding capacity `W`. `dp[j]` is the best value for capacity `j`, updated backwards as `dp[j] = max(dp[j], dp[j - w] + v)`. The answer may exceed the signed 32-bit range, so store DP values in 64-bit integers.

### Input

The first line contains two integers `n` and `W` (`1 <= n <= 200`, `0 <= W <= 20000`).

The next `n` lines each contain two integers `w_i` and `v_i` (`1 <= w_i <= 20000`, `1 <= v_i <= 1000000`). Items heavier than `W` cannot be chosen.

### Output

One integer: the maximum total value.

### Example

Input:

```text
3 5
2 3
3 4
4 5
```

Output:

```text
7
```

Choose the first two items: total weight `5`, total value `7`.

### Constraints

- Each item is chosen either zero or one time.
- The answer can exceed the range of a signed 32-bit integer; store DP values in a 64-bit integer.
- Use only the standard library.
