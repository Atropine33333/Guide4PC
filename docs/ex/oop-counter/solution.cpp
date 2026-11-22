// 参考实现：oop-counter
class Counter {
public:
    Counter();
    explicit Counter(int start);

    void increment();
    void decrement();
    int value() const;
    void reset();

    Counter& operator++();
    Counter operator++(int);
    Counter& operator+=(int amount);

private:
    int value_;
};

Counter::Counter() : value_(0) {}

Counter::Counter(int start) : value_(start < 0 ? 0 : start) {}

void Counter::increment() {
    ++value_;
}

void Counter::decrement() {
    if (value_ > 0) --value_;
}

int Counter::value() const {
    return value_;
}

void Counter::reset() {
    value_ = 0;
}

Counter& Counter::operator++() {
    ++value_;
    return *this;
}

Counter Counter::operator++(int) {
    Counter old(*this);
    ++value_;
    return old;
}

Counter& Counter::operator+=(int amount) {
    value_ += amount;
    if (value_ < 0) value_ = 0;
    return *this;
}
