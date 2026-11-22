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

Counter::Counter(int start) : value_(start) {
    // TODO: 负数取 0。
}

void Counter::increment() {
    // TODO: 加 1。
}

void Counter::decrement() {
    // TODO: 减 1，但不低于 0。
}

int Counter::value() const {
    return value_;
}

void Counter::reset() {
    // TODO: 置 0。
}

Counter& Counter::operator++() {
    // TODO: 前置自增。
    return *this;
}

Counter Counter::operator++(int) {
    // TODO: 后置自增，返回旧值。
    return *this;
}

Counter& Counter::operator+=(int amount) {
    (void)amount;
    // TODO: 加上 amount，截断到 0，返回 *this。
    return *this;
}
