This task reads no stdin and writes no stdout. Complete the five functions below; a hidden harness will test them.

```cpp
#include <cstddef>
#include <string>

std::size_t array_bytes(const int (&values)[6]);
std::size_t pointer_bytes(const int* values);
std::string which_overload(int (&values)[6]);
std::string which_overload(const int* values);
int sum_array(const int (&values)[6]);
int sum_range(const int* first, const int* last);
```

Details:

- The parameter of `array_bytes` is a reference to an array of six `int`, so `sizeof(values)` is the size of the whole array. Return it directly.
- The parameter of `pointer_bytes` is a pointer; an array decays when passed to it. Therefore `sizeof(values)` is the size of the pointer itself.
- The two `which_overload` functions form an overload set. An array argument (including a named array of length 6) selects the array-reference overload and returns `"array"`. A pointer argument such as `values + 0` or `&values[0]` selects the pointer overload and returns `"pointer"`.
- `sum_array` returns the sum of the six elements.
- `sum_range` returns the sum of the elements in `[first, last)`, or 0 when `first == last`.
- Do not write `main`, and do not change the signatures.
