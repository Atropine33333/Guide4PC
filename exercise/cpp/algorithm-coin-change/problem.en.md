**Chapter 18**

The kingdom of heaven is likened unto a certain king, which would take account of his servants. And the servant owed him `target` pieces of silver; and the king said, "Pay thou the debt with the fewest coins."

So the servant brought forth `m` kinds of coin, each in abundance. He grasped the greatest first, and the king shook his head; then the lesser, and still the king shook his head. For greed could not bring him to the fewest.

And the king said, "Whoso cannot pay, let him be delivered to the tormentors till the debt be paid." And he that could not pay outputteth `-1`.

**In plain words**: given `m` denominations and a `target`, each coin usable any number of times, output the fewest coins that sum exactly to `target`; if impossible, output `-1` (`0` when `target = 0`).

**Input**:

- The first line contains two integers `m` and `target` (`1 ≤ m ≤ 10`, `0 ≤ target ≤ 5000`).
- The second line contains `m` distinct positive integers, the denominations, each at most `1000`.

**Output**:

- One integer: the fewest coins; if the target cannot be made exactly, output `-1`.
- Output `0` when `target = 0`.

### Example

Input:

```text
3 6
1 3 4
```

Output:

```text
2
```

Two coins of denomination `3`; a greedy `4` first would give `4 + 1 + 1`, three coins.

### Constraints

- Unlimited coins of each denomination.
- Denominations are distinct, in any order.
- Only the minimum count is required, not the choice of coins.
- Output `-1` when impossible—never `0` or a large sentinel.
