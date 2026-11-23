Huizi said to Zhuangzi: "When the Peng journeys to the Southern Sea, it beats the water for three thousand li and rises on a whirlwind to ninety thousand li. Here is a flock of small birds, each carrying a word; the words are all in lowercase and of uneven length. I want them to fly in ranks, the earlier word ahead and the later word behind, in dictionary order, one bird to a line. How should this be done?"

Zhuangzi said: "Do you not see the wind? Once it rises, every hollow sounds at once, the large ones loudly and the small ones softly, each finding its place without being compared one by one. Ordering has such a long wind too, and its name is `qsort`: whoever borrows its force sees every word fall into order at a single call. When two words must be compared, hand them to `strcmp`, and one word settles which goes first."

Huizi said: "And what of words that are the same?"

Zhuangzi said: "Let identical words stand side by side, each on its own line; there is no need to remove any."

**In plain words**: read `n` words made of lowercase letters, sort them in dictionary order from smallest to largest, and print one word per line. Keep every occurrence of a repeated word.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 200`).
- The next `n` lines each contain one word of length 1 to 20, made only of lowercase letters `a`–`z`, with no spaces.

**Output**: `n` lines, one word each, sorted in dictionary order from smallest to largest.

### Example

Input:

```text
5
pear
apple
fig
apple
banana
```

Output:

```text
apple
apple
banana
fig
pear
```

### Contract

- The sort must call `qsort`, and comparing two words must call `strcmp`; do not write a sorting loop by hand.
- Dictionary order is the order `strcmp` gives; for lowercase words it matches the usual dictionary order.
- Do not remove duplicates: a word that appears several times is printed on several lines.
- Print only the word on each line, with no trailing spaces.
