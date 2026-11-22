// 参考实现：oop-stream-money
#include <cctype>
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>

class Money {
public:
    Money() = default;
    explicit Money(long long cents) : cents_(cents) {}

    long long cents() const { return cents_; }

    Money operator+(const Money& other) const { return Money(cents_ + other.cents_); }
    Money operator-(const Money& other) const { return Money(cents_ - other.cents_); }
    Money operator*(int factor) const { return Money(cents_ * factor); }

    bool operator==(const Money& other) const { return cents_ == other.cents_; }
    bool operator<(const Money& other) const { return cents_ < other.cents_; }

private:
    long long cents_ = 0;
};

std::ostream& operator<<(std::ostream& out, const Money& money) {
    const long long cents = money.cents();
    const bool negative = cents < 0;
    const long long magnitude = negative ? -cents : cents;
    if (negative) out << '-';
    out << '$' << magnitude / 100 << '.';
    const long long fraction = magnitude % 100;
    if (fraction < 10) out << '0';
    out << fraction;
    return out;
}

std::istream& operator>>(std::istream& in, Money& money) {
    std::string token;
    if (!(in >> token)) return in;

    std::size_t i = 0;
    bool negative = false;
    if (i < token.size() && token[i] == '-') {
        negative = true;
        ++i;
    }
    if (i < token.size() && token[i] == '$') ++i;

    const std::size_t digits_start = i;
    long long whole = 0;
    while (i < token.size() && std::isdigit(static_cast<unsigned char>(token[i]))) {
        whole = whole * 10 + (token[i] - '0');
        ++i;
    }
    if (i == digits_start) {
        in.setstate(std::ios::failbit);
        return in;
    }

    int fraction = 0;
    int fraction_digits = 0;
    if (i < token.size() && token[i] == '.') {
        ++i;
        while (i < token.size() && std::isdigit(static_cast<unsigned char>(token[i])) &&
               fraction_digits < 2) {
            fraction = fraction * 10 + (token[i] - '0');
            ++fraction_digits;
            ++i;
        }
        if (fraction_digits == 0) {
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    if (i != token.size()) {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (fraction_digits == 1) fraction *= 10;

    const long long cents = whole * 100 + fraction;
    money = Money(negative ? -cents : cents);
    return in;
}
