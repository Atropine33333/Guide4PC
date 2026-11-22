#include <vector>

class Polynomial {
public:
    Polynomial() = default;
    explicit Polynomial(std::vector<long long> coefficients)
        : coefficients_(coefficients) {}

    int degree() const {
        // TODO: trim high zero coefficients first.
        return static_cast<int>(coefficients_.size()) - 1;
    }

    long long coefficient(int power) const {
        if (power < 0 || static_cast<std::size_t>(power) >= coefficients_.size()) {
            return 0;
        }
        return coefficients_[static_cast<std::size_t>(power)];
    }

    long long evaluate(long long x) const {
        long long result = 0;
        for (std::size_t i = coefficients_.size(); i > 0; --i) {
            result = result * x + coefficients_[i - 1];
        }
        return result;
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial result = *this;
        if (result.coefficients_.size() < other.coefficients_.size()) {
            result.coefficients_.resize(other.coefficients_.size(), 0);
        }
        for (std::size_t i = 0; i < other.coefficients_.size(); ++i) {
            result.coefficients_[i] += other.coefficients_[i];
        }
        return result;
    }

    Polynomial operator*(const Polynomial& other) const {
        // TODO: compute the convolution.
        (void)other;
        return Polynomial();
    }

    bool operator==(const Polynomial& other) const {
        return coefficients_ == other.coefficients_;
    }

private:
    std::vector<long long> coefficients_;
};
