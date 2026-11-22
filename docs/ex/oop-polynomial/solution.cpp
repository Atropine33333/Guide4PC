// Reference solution: oop-polynomial
#include <cstddef>
#include <utility>
#include <vector>

class Polynomial {
public:
    Polynomial() = default;

    explicit Polynomial(std::vector<long long> coefficients)
        : coefficients_(std::move(coefficients)) {
        trim();
    }

    int degree() const {
        return coefficients_.empty()
                   ? -1
                   : static_cast<int>(coefficients_.size()) - 1;
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
        Polynomial result;
        const std::size_t size =
            coefficients_.size() > other.coefficients_.size()
                ? coefficients_.size()
                : other.coefficients_.size();
        result.coefficients_.resize(size, 0);
        for (std::size_t i = 0; i < size; ++i) {
            result.coefficients_[i] = coefficient(static_cast<int>(i)) +
                                      other.coefficient(static_cast<int>(i));
        }
        result.trim();
        return result;
    }

    Polynomial operator*(const Polynomial& other) const {
        if (coefficients_.empty() || other.coefficients_.empty()) {
            return Polynomial();
        }
        Polynomial result;
        result.coefficients_.assign(
            coefficients_.size() + other.coefficients_.size() - 1, 0);
        for (std::size_t i = 0; i < coefficients_.size(); ++i) {
            for (std::size_t j = 0; j < other.coefficients_.size(); ++j) {
                result.coefficients_[i + j] += coefficients_[i] *
                                                other.coefficients_[j];
            }
        }
        result.trim();
        return result;
    }

    bool operator==(const Polynomial& other) const {
        return coefficients_ == other.coefficients_;
    }

private:
    void trim() {
        while (!coefficients_.empty() && coefficients_.back() == 0) {
            coefficients_.pop_back();
        }
    }

    std::vector<long long> coefficients_;
};
