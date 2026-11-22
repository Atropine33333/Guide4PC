class Clock {
public:
    Clock(int hour, int minute, int second)
        : hour_(hour), minute_(minute), second_(second) {}

    void advance(int seconds) {
        // TODO: 按 24 小时循环进位，并支持负的 seconds。
        second_ += seconds;
    }

    int hour() const { return hour_; }
    int minute() const { return minute_; }
    int second() const { return second_; }

    int total_seconds() const {
        // TODO: 返回归一化后的秒数。
        return hour_ * 3600 + minute_ * 60 + second_;
    }

    bool operator==(const Clock& other) const {
        // TODO: 比较两个时刻（字段可能都还没有归一化）。
        return hour_ == other.hour_ && minute_ == other.minute_ && second_ == other.second_;
    }

private:
    int hour_ = 0;
    int minute_ = 0;
    int second_ = 0;
};
