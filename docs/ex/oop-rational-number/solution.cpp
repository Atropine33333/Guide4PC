// 参考实现：oop-rational-number
#include <numeric>

class Rational {
public:
    Rational(int numerator, int denominator)
        : numerator_(numerator), denominator_(denominator) {
        normalize();
    }

    int numerator() const { return numerator_; }
    int denominator() const { return denominator_; }

    Rational operator+(const Rational& other) const {
        return make(static_cast<long long>(numerator_) * other.denominator_ +
                        static_cast<long long>(other.numerator_) * denominator_,
                    static_cast<long long>(denominator_) * other.denominator_);
    }

    Rational operator-(const Rational& other) const {
        return make(static_cast<long long>(numerator_) * other.denominator_ -
                        static_cast<long long>(other.numerator_) * denominator_,
                    static_cast<long long>(denominator_) * other.denominator_);
    }

    Rational operator*(const Rational& other) const {
        return make(static_cast<long long>(numerator_) * other.numerator_,
                    static_cast<long long>(denominator_) * other.denominator_);
    }

    Rational operator/(const Rational& other) const {
        return make(static_cast<long long>(numerator_) * other.denominator_,
                    static_cast<long long>(denominator_) * other.numerator_);
    }

    bool operator==(const Rational& other) const {
        return numerator_ == other.numerator_ && denominator_ == other.denominator_;
    }

    bool operator<(const Rational& other) const {
        return static_cast<long long>(numerator_) * other.denominator_ <
               static_cast<long long>(other.numerator_) * denominator_;
    }

private:
    static Rational make(long long numerator, long long denominator) {
        return Rational(static_cast<int>(numerator), static_cast<int>(denominator));
    }

    void normalize() {
        if (denominator_ < 0) {
            denominator_ = -denominator_;
            numerator_ = -numerator_;
        }
        const int divisor = std::gcd(numerator_ < 0 ? -numerator_ : numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
        if (numerator_ == 0) denominator_ = 1;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};
