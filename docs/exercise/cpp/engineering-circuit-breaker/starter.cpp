class CircuitBreaker {
public:
    enum class State { Closed, Open, HalfOpen };

    CircuitBreaker(int failure_threshold, int cooldown_ticks)
        : threshold_(failure_threshold), cooldown_(cooldown_ticks) {
        (void)threshold_;
        (void)cooldown_;
    }

    bool allow() const {
        // TODO: 永远放行。
        return true;
    }

    void record(bool success) {
        (void)success;
        // TODO: 没有累计失败，也不会熔断。
    }

    void tick() {
        // TODO: 没有冷却计数。
    }

    State state() const {
        // TODO: 永远是 Closed。
        return State::Closed;
    }

    int failures() const {
        // TODO: 永远没有失败。
        return 0;
    }

private:
    int threshold_ = 0;
    int cooldown_ = 0;
};
