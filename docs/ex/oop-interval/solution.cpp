// 参考实现：oop-interval
#include <algorithm>

class Interval {
public:
    Interval(int low, int high) : low_(low), high_(high) {
        if (low_ > high_) {
            const int temp = low_;
            low_ = high_;
            high_ = temp;
        }
    }

    int low() const { return low_; }
    int high() const { return high_; }

    bool contains(int value) const {
        return low_ <= value && value <= high_;
    }

    bool overlaps(const Interval& other) const {
        return !(high_ < other.low_ || other.high_ < low_);
    }

    Interval united(const Interval& other) const {
        return Interval(std::min(low_, other.low_), std::max(high_, other.high_));
    }

    bool operator==(const Interval& other) const {
        return low_ == other.low_ && high_ == other.high_;
    }

private:
    int low_ = 0;
    int high_ = 0;
};
