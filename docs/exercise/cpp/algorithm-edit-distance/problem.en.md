**Chapter Eighteen**

The kingdom of heaven is like a man who held two words, one called `a` and one called `b`, each owing the other. The master said, "Make `a` into `b`. Each letter changed is a penny repaid; each letter added, each letter struck out, each letter swapped, counts as a penny also. Reckon the least you can pay."

So the man set the words letter against letter and wrote the account on a sheet, one row across and one column down. Then the master asked, "And how many letters do the two words hold in common, keeping their order?" The man answered, "That is a second ledger; it shall be reckoned too."

**In plain words**: implement two dynamic-programming functions: `edit_distance` returns the minimum number of single-character operations that turn `a` into `b` (insertion, deletion, and substitution each count as one step), and `longest_common_subsequence` returns the length of their longest common subsequence. Do not write `main`.

Required interface (do not write `main`):

```cpp
int edit_distance(const std::string& a, const std::string& b);
int longest_common_subsequence(const std::string& a, const std::string& b);
```

`edit_distance` returns the minimum number of single-character insertions, deletions, or substitutions that turn `a` into `b`. `longest_common_subsequence` returns the length of the longest common subsequence of the two strings.

### Input

None.

### Output

None.

### Example

```cpp
edit_distance("kitten", "sitting") == 3
longest_common_subsequence("kitten", "sitting") == 4
```

`kitten` becomes `sitting` by replacing `k` with `s`, replacing `e` with `i`, and appending `g`: three operations.

### Constraints

- Both strings have length at most 200.
- Strings contain lowercase English letters only.
- Fixed-seed random pairs check symmetry and triangle-inequality properties; your implementation must be deterministic.
