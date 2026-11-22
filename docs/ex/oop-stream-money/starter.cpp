#include <istream>
#include <ostream>

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
    // TODO: 还没有格式化输出。
    out << money.cents();
    return out;
}

std::istream& operator>>(std::istream& in, Money& money) {
    // TODO: 还没有解析输入，也没有处理非法记号。
    long long cents = 0;
    if (in >> cents) money = Money(cents);
    return in;
}
