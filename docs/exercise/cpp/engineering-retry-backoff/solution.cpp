// 参考实现：engineering-retry-backoff
#include <algorithm>

class RetryPolicy {
public:
    RetryPolicy(int base_delay, int max_delay, int max_attempts)
        : base_delay_(base_delay),
          max_delay_(max_delay),
          max_attempts_(max_attempts) {}

    bool has_next() const {
        return attempts_ < max_attempts_;
    }

    int next_delay() {
        if (!has_next()) return -1;
        long long delay = base_delay_;
        for (int i = 0; i < attempts_; ++i) {
            delay *= 2;
            if (delay >= max_delay_) {
                delay = max_delay_;
                break;
            }
        }
        delay = std::min(delay, static_cast<long long>(max_delay_));
        ++attempts_;
        return static_cast<int>(delay);
    }

    int attempts() const {
        return attempts_;
    }

    void reset() {
        attempts_ = 0;
    }

private:
    int base_delay_ = 0;
    int max_delay_ = 0;
    int max_attempts_ = 0;
    int attempts_ = 0;
};
