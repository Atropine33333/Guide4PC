enum class State { Created, Paid, Shipped, Delivered, Cancelled, Returned };

enum class Event { Pay, Ship, Deliver, Cancel, Return };
// 参考实现：engineering-state-machine
class Order {
public:
    Order() = default;

    State state() const { return state_; }
    int rejected() const { return rejected_; }

    bool apply(Event event) {
        State next = state_;
        switch (state_) {
            case State::Created:
                if (event == Event::Pay) next = State::Paid;
                if (event == Event::Cancel) next = State::Cancelled;
                break;
            case State::Paid:
                if (event == Event::Ship) next = State::Shipped;
                if (event == Event::Cancel) next = State::Cancelled;
                break;
            case State::Shipped:
                if (event == Event::Deliver) next = State::Delivered;
                break;
            case State::Delivered:
                if (event == Event::Return) next = State::Returned;
                break;
            case State::Cancelled:
            case State::Returned:
                break;
        }
        if (next == state_) {
            ++rejected_;
            return false;
        }
        state_ = next;
        return true;
    }

    static const char* name(State state) {
        switch (state) {
            case State::Created:
                return "created";
            case State::Paid:
                return "paid";
            case State::Shipped:
                return "shipped";
            case State::Delivered:
                return "delivered";
            case State::Cancelled:
                return "cancelled";
            case State::Returned:
                return "returned";
        }
        return "unknown";
    }

private:
    State state_ = State::Created;
    int rejected_ = 0;
};
