Implement a money class stored in cents and make it work with stream input and output. Complete this interface:

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

The judge includes your implementation and uses `std::istringstream` / `std::ostringstream`. **Do not write `main`.**

### Input

None.

### Output

None.

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
