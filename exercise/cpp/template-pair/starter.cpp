template <typename T>
class Pair {
public:
    Pair() = default;

    Pair(const T& first, const T& second) : first_(first), second_(second) {}

    T& first() { return first_; }

    const T& first() const { return first_; }

    T& second() { return second_; }

    const T& second() const { return second_; }

    Pair<T> swapped() const {
        // TODO: 返回交换两个元素后的新数对。
        return Pair<T>();
    }

    bool operator==(const Pair<T>& other) const {
        (void)other;
        // TODO: 两个元素都相等才返回 true。
        return false;
    }

private:
    T first_{};
    T second_{};
};
