// 参考实现：syntax-clock-wrap
class Clock {
public:
    Clock(int hour, int minute, int second)
        : total_(normalize(static_cast<long long>(hour) * 3600 + static_cast<long long>(minute) * 60 + second)) {}

    void advance(int seconds) {
        total_ = normalize(total_ + seconds);
    }

    int hour() const { return static_cast<int>(total_ / 3600); }
    int minute() const { return static_cast<int>((total_ / 60) % 60); }
    int second() const { return static_cast<int>(total_ % 60); }

    int total_seconds() const { return static_cast<int>(total_); }

    bool operator==(const Clock& other) const { return total_ == other.total_; }

private:
    static long long normalize(long long value) {
        value %= 86400;
        if (value < 0) value += 86400;
        return value;
    }

    long long total_ = 0;
};
