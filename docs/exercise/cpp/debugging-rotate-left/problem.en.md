Welcome to the Left-Rotation Corridor.

Every rule here is true. The corridor does not lie; when you hand it a negative number, it simply brings out a bigger hand to catch it.

Rule One: `size_t` in this corridor has no minus sign. Hand it `-1` and it hands back a number past eighteen quintillion. Where that lands after a modulo is anybody's guess.

Rule Two: to let a negative through, send it home to the positive side first. Before or after the modulo, bring `k` into `0` to `n - 1`; the corridor honours that range.

Rule Three: a vector of length 0 is a locked door. No rotation happens, and nothing behind it can be lost.

Rule Four: a one-element vector is the same person inside and outside the door. Rotate it as often as you like and it is still him.

Rule Five: the corridor's echo answers only to `values`. When the function returns, the caller's vector must already be rotated.

Rule Six: the door opens from the outside. Do not write `main`, and do not change the signature.

If you catch yourself computing "eighteen quintillion modulo five", put the pen down and read Rule Two again.

**In plain words**: implement `rotate_left(values, k)`: rotate left when `k` is positive and right when it is negative, with `|k|` possibly huge. Reduce the shift into `[0, n)` first: after taking the modulo by `n`, add `n` if the result is negative. Return an empty or single-element vector unchanged. The starter casts `k` straight to `size_t` and then takes the modulo, which turns negatives into astronomical values; fix that arithmetic and do not write `main`.

```cpp
void rotate_left(std::vector<int>& values, int k) {
    const std::size_t n = values.size();
    if (n == 0) return;
    const std::size_t shift = static_cast<std::size_t>(k) % n;
    std::vector<int> rotated(values.begin() + static_cast<std::ptrdiff_t>(shift), values.end());
    rotated.insert(rotated.end(), values.begin(),
                   values.begin() + static_cast<std::ptrdiff_t>(shift));
    values = std::move(rotated);
}
```

**Requirements**:

- `{1,2,3,4,5}` rotated left twice becomes `{3,4,5,1,2}`;
- `k == -1` means rotating right once;
- the shift must be reduced modulo the vector length;
- an empty vector never changes, and a one-element vector never changes.

**Do not write `main`.**

### Input

None.

### Output

None.

### Example

```cpp
std::vector<int> a{1, 2, 3, 4, 5};
rotate_left(a, 2);   // {3, 4, 5, 1, 2}
rotate_left(a, -1);  // {2, 3, 4, 5, 1}
```

### Constraints

- The vector has at most `2 * 10^5` elements.
- `k` fits in `int`; do not depend on assumptions beyond `long long` arithmetic.
