class Interval {
public:
    Interval(int low, int high) : low_(low), high_(high) {
        // TODO: 没有把端点顺序归一化。
    }

    int low() const { return low_; }
    int high() const { return high_; }

    bool contains(int value) const {
        // TODO: 写成了开区间。
        return low_ < value && value < high_;
    }

    bool overlaps(const Interval& other) const {
        (void)other;
        // TODO: 还没有判断重叠。
        return false;
    }

    Interval united(const Interval& other) const {
        (void)other;
        // TODO: 只返回了自己。
        return *this;
    }

    bool operator==(const Interval& other) const {
        return low_ == other.low_ && high_ == other.high_;
    }

private:
    int low_ = 0;
    int high_ = 0;
};
