// 参考实现：lifetime-ledger
class Tracked {
public:
    Tracked() { ++alive_; }

    explicit Tracked(int value) : value_(value) { ++alive_; }

    Tracked(const Tracked& other) : value_(other.value_) { ++alive_; }

    Tracked& operator=(const Tracked& other) {
        if (this != &other) {
            value_ = other.value_;
        }
        return *this;
    }

    ~Tracked() { --alive_; }

    int value() const { return value_; }

    void setValue(int value) { value_ = value; }

    static int alive() { return alive_; }

private:
    int value_ = 0;
    static int alive_;
};

int Tracked::alive_ = 0;
