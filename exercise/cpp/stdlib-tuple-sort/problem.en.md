The Code of Record Ordering

Article 1 (Purpose) This Code is enacted to fix the order among records and to prevent comparison functions from violating the strict weak ordering.

Article 2 (Score) A larger `score` comes first.

Article 3 (Name) For equal scores, a smaller `name` comes first in lexicographic order; the comparison uses `std::string`'s `<`, byte by byte.

Article 4 (Id) When both score and name are equal, a smaller `id` comes first.

Article 5 (Strict Weak Ordering) `record_less` must satisfy: `record_less(x, x)` is `false`; and for any two distinct records, exactly one of `record_less(x, y)` and `record_less(y, x)` is true.

Article 6 (In-Place Sorting) `sort_records` sorts in place by the rules above and may use `std::sort` together with `record_less`. Empty vectors and single-element vectors must also be handled.

Article 7 (Supplementary) Implement the interface below and do not write `main`. The score is an ordinary `int`; if a negated score is used to build a `std::tuple`, beware of `INT_MIN` and handle it separately.

```cpp
#include <string>
#include <vector>

struct Record {
    std::string name;
    int score;
    int id;
};

bool record_less(const Record& lhs, const Record& rhs);
void sort_records(std::vector<Record>& records);
```

**In plain words**: Implement `record_less` and `sort_records`. The order is: larger `score` first; for equal scores, smaller `name` first in lexicographic (byte-wise) order; if still equal, smaller `id` first. `record_less` must be a strict weak ordering: `record_less(x, x)` is `false`, and for two distinct records exactly one direction is true. `sort_records` sorts in place and may use `std::sort`; empty vectors and single-element vectors must work. Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `record_less({"a", 10, 1}, {"b", 10, 2})` | `true` |
| `record_less({"a", 10, 2}, {"a", 10, 2})` | `false` |

### Hint

Three nested `if`s work, or build `std::tuple(-score, name, id)` and compare with `operator<`. Negating the score overflows for `INT_MIN`, so handle that case separately.
