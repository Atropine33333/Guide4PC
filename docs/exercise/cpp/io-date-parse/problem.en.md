This task reads no stdin and writes no stdout. Implement strict ISO date parsing and short-format output.

```cpp
#include <string>

struct Date {
    int year;
    int month;
    int day;
};

bool parse_iso_date(const std::string& text, Date& out);

std::string format_short_date(const Date& date);
```

Rules:

- `parse_iso_date` accepts exactly the shape `YYYY-MM-DD`:
  - the length must be exactly 10;
  - characters 5 and 8 (1-based) must be `-`;
  - every other character must be an ASCII digit;
  - the year must be from 1 through 9999, the month from 1 through 12, and the day must exist in that month.
- Leap-year rule: a year divisible by 4 but not by 100 is a leap year; a year divisible by 400 is also a leap year. February has 29 days in a leap year and 28 otherwise.
- On success store the fields in `out` and return `true`. On failure return `false` and leave `out` **unchanged**.
- `format_short_date` returns `DD/MM/YYYY` with two digits for the day and month and four for the year, for example `2024-02-09` becomes `09/02/2024`. You may assume the supplied date is valid.
- Do not write `main`, and do not change the struct or the signatures.
