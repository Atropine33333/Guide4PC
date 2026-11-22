// 参考实现：memory-shared-ref
class SharedInt {
public:
    explicit SharedInt(int value = 0) : control_(new Control{value, 1}) {}

    SharedInt(const SharedInt& other) : control_(other.control_) {
        ++control_->count;
    }

    SharedInt& operator=(const SharedInt& other) {
        if (this != &other) {
            other.control_->count++;
            release();
            control_ = other.control_;
        }
        return *this;
    }

    ~SharedInt() {
        release();
    }

    int value() const {
        return control_->value;
    }

    void set_value(int value) {
        control_->value = value;
    }

    int use_count() const {
        return control_->count;
    }

private:
    struct Control {
        int value;
        int count;
    };

    void release() {
        if (--control_->count == 0) delete control_;
    }

    Control* control_ = nullptr;
};
