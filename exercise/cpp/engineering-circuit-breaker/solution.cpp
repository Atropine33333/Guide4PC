// 参考实现：engineering-circuit-breaker
class CircuitBreaker {
public:
    enum class State { Closed, Open, HalfOpen };

    CircuitBreaker(int failure_threshold, int cooldown_ticks)
        : threshold_(failure_threshold), cooldown_(cooldown_ticks) {}

    bool allow() const {
        return state_ != State::Open;
    }

    void record(bool success) {
        if (state_ == State::Open) return;

        if (success) {
            failures_ = 0;
            state_ = State::Closed;
            remaining_ = 0;
            return;
        }

        ++failures_;
        if (state_ == State::HalfOpen || failures_ >= threshold_) {
            trip();
        }
    }

    void tick() {
        if (state_ != State::Open) return;
        if (remaining_ > 0) --remaining_;
        if (remaining_ == 0) {
            state_ = State::HalfOpen;
        }
    }

    State state() const {
        return state_;
    }

    int failures() const {
        return failures_;
    }

private:
    void trip() {
        if (cooldown_ == 0) {
            state_ = State::HalfOpen;
            remaining_ = 0;
        } else {
            state_ = State::Open;
            remaining_ = cooldown_;
        }
    }

    int threshold_ = 1;
    int cooldown_ = 0;
    int failures_ = 0;
    int remaining_ = 0;
    State state_ = State::Closed;
};
