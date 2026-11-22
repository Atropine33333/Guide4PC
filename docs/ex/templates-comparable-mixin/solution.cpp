// 参考实现：templates-comparable-mixin
#include <string>
#include <utility>

template <typename Derived>
struct Comparable {
    friend bool operator!=(const Derived& lhs, const Derived& rhs) { return !(lhs == rhs); }
    friend bool operator>(const Derived& lhs, const Derived& rhs) { return rhs < lhs; }
    friend bool operator<=(const Derived& lhs, const Derived& rhs) { return !(rhs < lhs); }
    friend bool operator>=(const Derived& lhs, const Derived& rhs) { return !(lhs < rhs); }
};

struct Entry : Comparable<Entry> {
    std::string name;
    int score = 0;
    int id = 0;

    Entry() = default;
    Entry(std::string n, int s, int i) : name(std::move(n)), score(s), id(i) {}

    bool operator<(const Entry& other) const {
        if (score != other.score) return score > other.score;
        return id < other.id;
    }

    bool operator==(const Entry& other) const {
        return score == other.score && id == other.id && name == other.name;
    }
};
