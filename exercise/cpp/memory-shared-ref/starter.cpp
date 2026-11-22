class SharedInt {
public:
    explicit SharedInt(int value = 0) : value_(new int(value)) {}

    SharedInt(const SharedInt& other) : value_(new int(*other.value_)) {
        // TODO: 拷贝应当共享数据并把引用计数加一。
    }

    SharedInt& operator=(const SharedInt& other) {
        // TODO: 释放旧数据，共享 other 的数据并更新计数。
        (void)other;
        return *this;
    }

    ~SharedInt() {
        delete value_;
    }

    int value() const {
        return *value_;
    }

    void set_value(int value) {
        *value_ = value;
    }

    int use_count() const {
        // TODO: 返回真实计数。
        return 1;
    }

private:
    int* value_ = nullptr;
};
