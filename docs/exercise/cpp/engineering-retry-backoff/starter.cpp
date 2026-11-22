class RetryPolicy {
public:
    RetryPolicy(int base_delay, int max_delay, int max_attempts)
        : base_delay_(base_delay), max_delay_(max_delay), max_attempts_(max_attempts) {
        (void)base_delay_;
        (void)max_delay_;
        (void)max_attempts_;
    }

    bool has_next() const {
        // TODO: 还没有判断剩余尝试次数。
        return false;
    }

    int next_delay() {
        // TODO: 总是返回 -1。
        return -1;
    }

    int attempts() const {
        return attempts_;
    }

    void reset() {
        // TODO: 没有清零。
    }

private:
    int base_delay_ = 0;
    int max_delay_ = 0;
    int max_attempts_ = 0;
    int attempts_ = 0;
};
