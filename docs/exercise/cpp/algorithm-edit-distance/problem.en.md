Implement two dynamic-programming functions over strings.

Required interface (do not write `main`):

```cpp
int edit_distance(const std::string& a, const std::string& b);
int longest_common_subsequence(const std::string& a, const std::string& b);
```

`edit_distance` returns the minimum number of single-character insertions, deletions, or substitutions that turn `a` into `b`. `longest_common_subsequence` returns the length of the longest common subsequence of the two strings.

### Input

None. The judge calls the functions directly.

### Output

None. The judge inspects the return values.

### Example

```cpp
edit_distance("kitten", "sitting") == 3
longest_common_subsequence("kitten", "sitting") == 4
```

`kitten` becomes `sitting` by replacing `k` with `s`, replacing `e` with `i`, and appending `g`: three operations.

### Constraints

- Both strings have length at most 200.
- Strings contain lowercase English letters only.
- The judge checks fixed-seed random pairs for symmetry and triangle-inequality properties; your implementation must be deterministic.
