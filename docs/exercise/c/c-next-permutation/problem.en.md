(On the bridge over the Hao, Zhuangzi and Huizi watch the fish.)

Huizi lines up a handful of bamboo slips. "This row has an order. Rearrange it, and you have another row."

Zhuangzi says, "Rows end, but meaning does not. Take a row to its very end, then step once more — and you have crossed over."

"Crossed over to what?" asks Huizi.

"To emptiness. At the end of the rows there is only emptiness, and no next row can be written."

**In plain words**: read `n` and a permutation of `1..n`, then print the permutation that comes immediately after it in dictionary order; if it is the last one, print `none`.

**Input**: the first line holds an integer `n` (`1 ≤ n ≤ 1000`); the second line holds `n` integers, a permutation of `1..n`, separated by single spaces.

**Output**: one line with the next permutation, numbers separated by single spaces; or `none` if there is no next permutation.

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

After `1 2 3` come `1 3 2`, then `2 1 3`, and so on; `3 2 1` is the last, so it has no successor.

### Contract

- Dictionary order compares position by position, starting with the first difference.
- The next permutation is the one immediately following the current permutation among all permutations.
- If the current permutation is strictly decreasing from left to right, it is the last one; print `none`.
- Separate numbers with single spaces and print no trailing space.
