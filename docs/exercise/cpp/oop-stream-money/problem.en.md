"Oh, my dear friend, there you are at last!" The bookkeeper pushed his spectacles up his nose. "This money, you see, is kept in cents — every last one of them accounted for."

"When we write it out," he said, lowering his voice, "first the dollar sign, then exactly two decimals. Twelve hundred and thirty-four cents goes out as `$12.34`; five cents as `$0.05`; five cents owed by us gets a minus in front, `-$0.05`; nothing at all is `$0.00`. Good heavens, not a single zero may go missing."

"Reading it in takes even more care: maybe a minus first, maybe a dollar sign, then a decimal point with one or two places after it. '`12.34`', '-`$0.05`', '`$3.25`', '`7`' — all fine; '`0.5`' means fifty cents. But if someone hands you '`abc`' or '`1.234`' — oh no, my friend, set the stream's `failbit` and do not touch the amount, not one cent!"

**In plain words**: Implement the money class `Money`, stored in cents: `+`, `-` and `*` work in cents, `==` and `<` compare amounts; `operator<<` writes a dollar sign and two decimals, and `operator>>` reads an optional minus, an optional `$`, at least one digit and an optional decimal point with one or two places. A malformed token sets `failbit` and leaves the object unchanged.

```cpp
class Money {
public:
    Money() = default;
    explicit Money(long long cents);

    long long cents() const;

    Money operator+(const Money& other) const;
    Money operator-(const Money& other) const;
    Money operator*(int factor) const;

    bool operator==(const Money& other) const;
    bool operator<(const Money& other) const;

private:
    long long cents_ = 0;
};

std::ostream& operator<<(std::ostream& out, const Money& money);
std::istream& operator>>(std::istream& in, Money& money);
```

The output format is a dollar sign and exactly two decimals: 1234 cents prints `$12.34`, 5 cents prints `$0.05`, -5 cents prints `-$0.05`, and 0 prints `$0.00`.

Input is one whitespace-delimited token with this grammar: optional `-`, optional `$`, at least one digit, and optionally `.` followed by one or two decimals. For example `12.34`, `-$0.05`, `$3.25`, and `7` are valid; `0.5` means 50 cents. An invalid token (such as `abc` or `1.234`) must set the stream's `failbit` and leave `money` unchanged.

### Example

```cpp
Money price;
std::istringstream("$3.25") >> price;   // price.cents() == 325
std::ostringstream out;
out << price;                            // "$3.25"
```

### Constraints

- Absolute values are at most `10^15` cents.
- Use `iostream`, not `printf`/`scanf`.
