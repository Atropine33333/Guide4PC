This exercise reads no stdin. Implement the ordering rule for records; a hidden harness calls it.

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

Ordering rules, applied in sequence:

1. larger `score` first;
2. for equal scores, smaller `name` first (use `std::string`'s `<`, a byte-wise comparison);
3. if still equal, smaller `id` first.

Requirements:

- `record_less` must be a strict weak ordering: `record_less(x, x)` is `false`, and for two distinct records exactly one of `record_less(x, y)` and `record_less(y, x)` is true.
- `sort_records` reorders in place by those rules; `std::sort` and `record_less` may be used.
- Empty vectors and single-element vectors must work.
- Do not write `main`.

### Hint

Three nested `if`s work, or build `std::tuple(-score, name, id)` and compare with `operator<`. Negating the score overflows for `INT_MIN`, so handle that case separately.
