// 参考实现：template-bounded-stack
template <typename T, int Capacity>
class BoundedStack {
public:
    bool push(const T& value) {
        if (size_ >= Capacity) return false;
        data_[size_++] = value;
        return true;
    }

    bool pop() {
        if (size_ == 0) return false;
        --size_;
        return true;
    }

    bool top(T& out) const {
        if (size_ == 0) return false;
        out = data_[size_ - 1];
        return true;
    }

    int size() const { return size_; }

    static constexpr int capacity() { return Capacity; }

private:
    T data_[Capacity]{};
    int size_ = 0;
};
