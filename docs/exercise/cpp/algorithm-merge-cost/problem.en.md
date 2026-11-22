A row of stone piles, where pile `i` holds `piles[i]` stones. In one move you merge two adjacent piles into one, paying the total number of stones in them; the merged pile stays in place. Merge everything into a single pile and minimize the total cost.

Required interface (do not write `main`):

```cpp
long long min_merge_cost(const std::vector<int>& piles);
```

### Input

None. The judge calls the function directly.

### Output

None. The function returns the minimum total cost, which fits in `long long`.

### Example

```cpp
min_merge_cost({1, 2, 3}) == 9
```

Merge `1` and `2` for 3, leaving `{3, 3}`; merge those for 6; total 9. Merging `2` and `3` first costs 5 + 6 = 11, which is worse.

### Constraints

- `0 <= piles.size() <= 100`
- `0 <= piles[i] <= 1000000`
- An empty array returns 0.
- The judge generates fixed-seed small inputs and compares against brute force over all merge orders.
