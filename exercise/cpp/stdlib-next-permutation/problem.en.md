Here are `n` numbers in a row, each with its value written down. The question: in lexicographic order, what is the row that follows this one?

The rule says: from right to left, find the first place where the earlier number is smaller than the later one. If such a place is found, again from right to left find the first number greater than that earlier number, exchange the two, then reverse all numbers after that earlier place; the next row is obtained. If no such place is found, the row is already descending from great to small and is the last row; reverse it into ascending order, and it becomes the first row again.

The questioner also asks: numbers may repeat; how shall that be handled? The answer: repeated numbers keep their positions, and the row after `1 1` is still `1 1`. Do not sort and deduplicate first to skip repeated rows.

**In plain words**: Read `n` integers and print their next permutation in lexicographic order; if the current sequence is already the last permutation (strictly descending), the next permutation is the first one (ascending). Permutations are compared as whole sequences, independent of element count; duplicate values take part by position, and equal elements follow the same "next" definition. You may call `std::next_permutation` or implement the rule yourself.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 20`).
- The second line contains `n` integers, each between `-10^9` and `10^9`, possibly with duplicates.

**Output**: one line with the next permutation, the integers separated by single spaces.

### Example

Input:

```text
3
1 2 3
```

Output:

```text
1 3 2
```

### Constraints

- Permutations are ordered lexicographically as whole sequences, independent of element count.
- Duplicate values take part by position; for example the next permutation of `1 1` is still `1 1`.
- Equal elements still follow the same "next" definition: do not sort and deduplicate to skip repeated permutations.
- You may call `std::next_permutation` or implement the algorithm yourself.
