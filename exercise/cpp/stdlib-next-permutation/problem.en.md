Read `n` integers and print their **next permutation** in lexicographic order. If the current sequence is already the last permutation (strictly descending), the next permutation is the first one (ascending).

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

- Permutations are ordered lexicographically as whole sequences.
- Duplicate values take part by position; for example the next permutation of `1 1` is still `1 1`.
- Equal elements still follow the same "next" definition: do not sort and deduplicate to skip repeated permutations.
- You may call `std::next_permutation` or implement the algorithm yourself.
