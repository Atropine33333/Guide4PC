class Tracked {
public:
    Tracked() { ++alive_; }

    explicit Tracked(int value) : value_(value) { ++alive_; }

    Tracked(const Tracked& other) : value_(other.value_) {
        // TODO: 拷贝构造函数也创建了一个新对象，存活数要加一。
    }

    Tracked& operator=(const Tracked& other) {
        // TODO: 复制值；自赋值要安全，存活数不变。
        value_ = other.value_;
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
