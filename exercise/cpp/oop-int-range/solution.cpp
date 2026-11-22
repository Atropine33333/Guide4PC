// Reference solution: oop-int-range
#include <cstddef>
#include <iterator>

class IntRange {
public:
    class iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = const int*;
        using reference = int;

        iterator() : value_(0) {}
        explicit iterator(int value) : value_(value) {}

        int operator*() const { return value_; }
        iterator& operator++() {
            ++value_;
            return *this;
        }
        iterator operator++(int) {
            iterator copy = *this;
            ++(*this);
            return copy;
        }
        bool operator==(const iterator& other) const {
            return value_ == other.value_;
        }
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

    private:
        int value_;
    };

    IntRange(int first, int stop) : first_(first), stop_(stop) {}

    iterator begin() const {
        return iterator(first_ < stop_ ? first_ : stop_);
    }
    iterator end() const { return iterator(stop_); }

private:
    int first_;
    int stop_;
};
