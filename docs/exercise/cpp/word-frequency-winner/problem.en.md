Given a number of lowercase English words, find the one that occurs most often. If several words share the highest count, output the one that is smallest in lexicographic order.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 1000`).
- The second line contains `n` words separated by single spaces. Each word consists of lowercase letters only and has at most 20 characters.

**Output**: one line with the winning word.

### Example

Input:

```text
5
apple banana apple cherry banana
```

Output:

```text
apple
```

`apple` and `banana` both occur twice, and `apple` is lexicographically smaller.

### Hint

`std::map<std::string, int>` counts occurrences and keeps the keys sorted; updating the answer only on a strictly greater count preserves the lexicographically smallest word on ties.
