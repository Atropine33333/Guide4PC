This exercise reads no stdin and writes no stdout. Implement two functions with default arguments; a hidden harness runs the tests.

```cpp
int clamp_value(int value, int low = 0, int high = 100);
std::string join_text(const std::vector<std::string>& parts,
                      const std::string& separator = ",");
```

Rules:

- `clamp_value` limits `value` to the closed interval `[low, high]`; if `low > high`, swap the bounds first; all three arguments may be negative.
- `join_text` concatenates the strings in `parts` with `separator` between neighbours; return an empty string for an empty `parts`; keep empty elements; `separator` may be empty.
- Write the default arguments only once, in the function definition; the harness calls `clamp_value` with one, two, and three arguments, and `join_text` with one and two arguments.
- Do not write `main`.

### Hint

Default arguments apply at the call site: omitted arguments are filled in from the declaration.
