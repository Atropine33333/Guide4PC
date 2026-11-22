**Chapter One**

In those days two camps were drawn up in the wilderness, each in its own order, from least to greatest, and none stepped ahead of its neighbour.

And the LORD spoke to Moses, saying:

"Take the men of these two camps and make them one camp. Look on the men of the left camp first, and on the men of the right camp after; whoever bears the smaller number goes out first. Where the two numbers are equal, the left camp goes out first, and the order shall not be changed. When one camp is emptied, the remnant of the other shall follow in its own order, and not one of them shall be lost."

And Moses did as the LORD commanded him, and the two camps became one, and not a man was missing.

But afterwards a man wrote a program after this command, and when the left camp was emptied he stayed his hand; and the remnant of the right camp remained in the wilderness and never entered the roll.

**In plain words**: read two non-decreasing sequences and print them merged into one non-decreasing sequence. Walk both with two pointers, taking the first sequence's element on equality (the merge must be stable); when one side runs out, append every remaining element of the other. Repair the loop in the starter that drops the tail and keep the input and output format.

**Input**:

- The first line holds two integers `n m` (`0 ≤ n, m ≤ 10^5`, `n + m ≥ 1`).
- The second line holds `n` integers in non-decreasing order; when `n = 0` this line may be empty.
- The third line holds `m` integers in non-decreasing order; when `m = 0` this line may be empty.

**Output**: one line with the merged non-decreasing sequence, separated by single spaces.

### Example

Input:

```text
2 3
1 4
2 5 6
```

Output:

```text
1 2 4 5 6
```

### Constraints

- Do not change the input/output format and do not rewrite it as a completely different interaction.
- Either sequence may be empty, and there may be many duplicate values.
- The merge must be stable: on equality take the element from the first sequence first.
- Only modify `starter.cpp`.
