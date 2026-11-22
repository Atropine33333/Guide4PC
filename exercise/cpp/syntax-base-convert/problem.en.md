> A number has no fixed shape; it takes the shape of its base.
> Divide by the base and keep the remainder, writing from the end;
> multiply by the base and add, reading from the front.

The *Nine Chapters* has a rule: every number written follows its own base, and the base is the count at which the writing rolls over. Here is one number, to be written in any base between two and thirty-six; and here is a writing left by another hand, to be turned back into a number. Both are done on the counting rods, without question and without answer.

**In plain words**: Implement two functions that convert integers between bases 2 and 36.

```cpp
#include <string>

std::string to_base(long long value, int base);
bool parse_base(const std::string& text, int base, long long& out);
```

**Rules**:

- The base range is `2 ≤ base ≤ 36`; outside it, `to_base` returns an empty string and `parse_base` returns `false`.
- `to_base` writes `value` in that base: digits above 9 use uppercase `A`–`Z` in order, `0` is written `"0"`, and a negative value carries one leading `-`.
- `to_base` must handle `LLONG_MIN` (`-9223372036854775808`); negating it directly is not allowed.
- `parse_base` accepts an optional `+` or `-` sign followed by at least one digit; letters in either case are accepted.
- If `text` contains an illegal character, contains no digit, or its value falls outside the `long long` range, return `false` and leave `out` untouched.
- On success `parse_base` writes the result into `out` and returns `true`.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `to_base(255, 16)` | `"FF"` |
| `to_base(-5, 2)` | `"-101"` |
| `to_base(0, 36)` | `"0"` |
| `parse_base("101", 2, out)` | `true`, `out == 5` |
| `parse_base("-ff", 16, out)` | `true`, `out == -255` |
| `parse_base("12", 2, out)` | `false` |

### The rule of the rods

First turn the negative value into the magnitude of an `unsigned long long` and take remainders digit by digit, then put the minus sign back once at the end. While parsing, accumulate in an `unsigned long long` and check before each multiplication whether it would pass `LLONG_MAX` (for a negative value the limit is `1ULL << 63`).
