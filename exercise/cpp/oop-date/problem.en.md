This exercise reads no stdin and writes no stdout. Implement a date class; a hidden harness runs the tests.

```cpp
class Date {
public:
    Date(int year, int month, int day);

    bool valid() const;
    void next_day();

    int year() const;
    int month() const;
    int day() const;

    static bool is_leap(int year);
    static int days_in_month(int year, int month);

    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
};
```

Rules:

- A valid date has year `1..9999`, month `1..12`, and day `1..days_in_month(year, month)`.
- `is_leap(y)`: divisible by 4 but not by 100, or divisible by 400.
- `days_in_month(y, m)`: return 0 for an invalid month; February has 29 or 28 days depending on the leap rule; other months have 30 or 31.
- `valid()` judges the current object; an invalid object keeps the three values it was constructed with.
- `next_day()` advances by one day; an invalid date stays unchanged; the day after `9999-12-31` is outside the supported year range, so it also stays unchanged.
- `<` and `==` compare year, then month, then day, and do not require a valid date.
- Do not write `main`.

### Hint

In `next_day`, increment `day` first. If it exceeds the month's length, reset `day` to 1 and increment the month; if the month exceeds 12, reset it to 1 and increment the year. Handle `9999-12-31` separately.
