enum class State { Created, Paid, Shipped, Delivered, Cancelled, Returned };

enum class Event { Pay, Ship, Deliver, Cancel, Return };
class Order {
public:
    Order() = default;

    State state() const { return state_; }
    int rejected() const { return rejected_; }

    bool apply(Event event) {
        // TODO: 只处理了 Created + Pay 这一条迁移。
        if (state_ == State::Created && event == Event::Pay) {
            state_ = State::Paid;
            return true;
        }
        return false;
    }

    static const char* name(State state) {
        // TODO: 只返回了一个名字。
        (void)state;
        return "created";
    }

private:
    State state_ = State::Created;
    int rejected_ = 0;
};
