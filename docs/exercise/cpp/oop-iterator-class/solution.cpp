#include <iterator>

class IntRange {
public:
    IntRange(int start, int stop, int step = 1)
        : start_(start), stop_(stop), step_(step) {}

    class iterator {
    public:
        iterator() : value_(0), stop_(0), step_(1), done_(true) {}

        iterator(int value, int stop, int step)
            : value_(value), stop_(stop), step_(step), done_(passed()) {}

        int operator*() const { return value_; }

        iterator& operator++() {
            value_ += step_;
            done_ = passed();
            return *this;
        }

        bool operator==(const iterator& other) const {
            if (done_ || other.done_) {
                return done_ == other.done_;
            }
            return value_ == other.value_;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

    private:
        bool passed() const {
            return step_ > 0 ? value_ >= stop_ : value_ <= stop_;
        }

        int value_;
        int stop_;
        int step_;
        bool done_;
    };

    iterator begin() const { return iterator(start_, stop_, step_); }

    iterator end() const { return iterator(); }

private:
    int start_;
    int stop_;
    int step_;
};
