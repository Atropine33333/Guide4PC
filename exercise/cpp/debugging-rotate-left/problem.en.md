The function below should rotate `values` left by `k` positions: a positive `k` rotates left, a negative `k` rotates right, and `|k|` may be large. It returns wrong results for some inputs. Fix it.

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

Requirements:

- `{1,2,3,4,5}` rotated left twice becomes `{3,4,5,1,2}`;
- `k == -1` means rotating right once;
- the shift must be reduced modulo the vector length;
- an empty vector never changes, and a one-element vector never changes.

The judge includes your implementation and checks these boundaries. **Do not write `main`.**

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
