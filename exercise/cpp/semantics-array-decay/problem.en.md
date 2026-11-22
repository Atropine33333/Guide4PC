NOTICE AT THE ARRAY OBSERVATION POST

There is no ticket gate here, and no fee is collected. Visitors shall read the rules posted on the wall before they begin. Every rule is true and remains in force; the rules sit uneasily together only because common sense does not apply at this site. The rules themselves contain no error.

**In plain words**: Complete the functions below: report the byte size of a whole array and of a pointer, tell which of two same-named overloads an argument selects, sum six elements, and sum a half-open range. The difference lives entirely in how the parameter is written, so copy the signatures exactly and then follow the rules.

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

### The rules

1. When a named array enters a function whose parameter is written as a pointer, it decays on the spot into a pointer to its first element, and its length is lost; `sizeof(values)` then measures the width of the pointer itself. `pointer_bytes` follows this rule.

2. When the parameter is written `const int (&)[6]`, the array enters the function intact, and its length arrives with its elements; `sizeof(values)` then measures the whole array. `array_bytes` follows this rule.

3. The two `which_overload` functions form an overload set. An array argument selects the array-reference version and returns `"array"`; a pointer argument selects the pointer version and returns `"pointer"`. The pointer may be written `values + 0` or `&values[0]`; there is no exception to this rule.

4. `sum_array` returns the sum of the six elements.

5. `sum_range` recognizes only the half-open range `[first, last)`; when the two ends are equal it returns 0 and offers no explanation.

6. Do not write `main`, and do not change the signatures.
