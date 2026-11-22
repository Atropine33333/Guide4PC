RULES OF THE CLAMPING INTERVAL

Article 1. This interval recognizes only "less than". A ruler, a word, an elephant — anything that can answer which of two is smaller may enter.

Article 2. If `value` is less than `low`, hand over `low`. If `high` is less than `value`, hand over `high`. If neither has happened, hand over `value` unchanged.

Article 3. The two ends are the caller's responsibility: `low` is not greater than `high`. This article is true, and need not be re-checked.

Article 4. When `low` and `high` are equal, one value remains in the interval. Hand it over. It is the left end and the right end at once; common sense is void here, and the rules govern.

Article 5. This interval does no arithmetic. Addition, subtraction, multiplication and division are all refused; no door opens except `<`. Requiring `T` to add is beyond our authority.

Article 6. The result is a `T`, returned by value. `T` visits at least as `int`, `double`, and `char`.

The rules end here. Every article is true. Follow them.

**In plain words**: implement the function template `clamp_value` using only `<`, clamping `value` into `[low, high]`; the caller guarantees `low <= high`. Write it header-style.

```cpp
template <typename T>
T clamp_value(const T& value, const T& low, const T& high);
```

Return:

- `low` if `value < low`;
- `high` if `high < value`;
- otherwise `value`.

Callers guarantee `low <= high`. `T` is instantiated at least with `int`, `double`, and `char`. Do not write `main`.

Requirements:

- Use only `<` comparisons; do not require `T` to support arithmetic;
- When `low == high`, only one value is in range and the result must be it;
- The return type is `T` (returned by value).
