#include <string>
#include <utility>

template <typename Derived>
struct Comparable {
    // TODO: 四个运算符都还没有实现。
    friend bool operator!=(const Derived&, const Derived&) { return false; }
    friend bool operator>(const Derived&, const Derived&) { return false; }
    friend bool operator<=(const Derived&, const Derived&) { return false; }
    friend bool operator>=(const Derived&, const Derived&) { return false; }
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
