class Rational {
public:
    Rational(int numerator, int denominator)
        : numerator_(numerator), denominator_(denominator) {
        // TODO: 约分并把分母的符号移到分子上。
    }

    int numerator() const { return numerator_; }
    int denominator() const { return denominator_; }

    Rational operator+(const Rational& other) const {
        // TODO: 相加并约分。
        return Rational(numerator_ + other.numerator_, denominator_ + other.denominator_);
    }

    Rational operator-(const Rational& other) const {
        // TODO: 相减并约分。
        return Rational(numerator_ - other.numerator_, denominator_ - other.denominator_);
    }

    Rational operator*(const Rational& other) const {
        // TODO: 相乘并约分。
        return Rational(numerator_ * other.numerator_, denominator_ * other.denominator_);
    }

    Rational operator/(const Rational& other) const {
        // TODO: 相除并约分。
        return Rational(numerator_ * other.denominator_, denominator_ * other.numerator_);
    }

    bool operator==(const Rational& other) const {
        // TODO: 按数值比较。
        return numerator_ == other.numerator_ && denominator_ == other.denominator_;
    }

    bool operator<(const Rational& other) const {
        // TODO: 不要用浮点数。
        return static_cast<double>(numerator_) / denominator_ <
               static_cast<double>(other.numerator_) / other.denominator_;
    }

private:
    int numerator_ = 0;
    int denominator_ = 1;
};
