#include <iterator>

class IntRange {
public:
    IntRange(int start, int stop, int step = 1)
        : start_(start), stop_(stop), step_(step) {}

    class iterator {
    public:
        int operator*() const {
            return 0;  // TODO
        }

        iterator& operator++() {
            // TODO
            return *this;
        }

        bool operator==(const iterator& other) const {
            (void)other;
            return true;  // TODO
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);  // TODO: 依赖正确的 ==
        }
    };

    iterator begin() const {
        // TODO
        (void)start_;
        (void)stop_;
        (void)step_;
        return iterator{};
    }

    iterator end() const {
        // TODO
        return iterator{};
    }

private:
    int start_;
    int stop_;
    int step_;
};
