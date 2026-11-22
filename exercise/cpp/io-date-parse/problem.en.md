*Standing Orders for the Night Shift of the Date Bureau*

You are the new night clerk. Every order below is true. Follow each one.

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

**Order 1 (Shape)** Only strings shaped exactly `YYYY-MM-DD` are admitted. The length must be exactly 10; characters 5 and 8 (1-based) must be `-`; every other character must be an ASCII digit. One short, one long, one shifted: none is admitted.

**Order 2 (Range)** The year lies from 1 through 9999, the month from 1 through 12, and the day must exist in that month. This looks like common sense. Remember: common sense holds only while no order speaks.

**Order 3 (Leap years)** Two sentences govern leap years. A year divisible by 4 and not by 100 is a leap year; a year divisible by 400 is also a leap year. The second speaks exactly where the first falls silent. February has 29 days in a leap year and 28 otherwise.

**Order 4 (The drawer)** On success, write the year, month and day into `out` and return `true`. On failure, return `false` and leave `out` **unchanged**. On a failed night, the drawer keeps what it held.

**Order 5 (Short form)** `format_short_date` returns `DD/MM/YYYY` with two digits for the day and the month, zero-padded, and four for the year; for example `2024-02-09` becomes `09/02/2024`. You may assume the supplied date is valid.

**Order 6 (Signature)** Do not write `main`, and do not change the struct or the signatures.

**In plain words**: implement `parse_iso_date` and `format_short_date`. The first parses an ISO date strictly by shape, range and the leap-year orders, leaving `out` untouched on failure; the second prints a valid date as `DD/MM/YYYY`.
