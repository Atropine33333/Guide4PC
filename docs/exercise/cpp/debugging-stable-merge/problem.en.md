The program below merges two non-decreasing sequences into one, but it loses elements for some inputs. Fix `starter.cpp` so that it is correct for every valid input.

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
