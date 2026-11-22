// 参考实现：template-pair
template <typename T>
class Pair {
public:
    Pair() = default;

    Pair(const T& first, const T& second) : first_(first), second_(second) {}

    T& first() { return first_; }

    const T& first() const { return first_; }

    T& second() { return second_; }

    const T& second() const { return second_; }

    Pair<T> swapped() const { return Pair<T>(second_, first_); }

    bool operator==(const Pair<T>& other) const {
        return first_ == other.first_ && second_ == other.second_;
    }

private:
    T first_{};
    T second_{};
};
