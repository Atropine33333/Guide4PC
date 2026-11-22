class UniqueIntArray {
public:
    explicit UniqueIntArray(int n) : size_(n), data_(n > 0 ? new int[n]{} : nullptr) {}

    ~UniqueIntArray() {
        delete[] data_;
    }

    UniqueIntArray(const UniqueIntArray&) = delete;
    UniqueIntArray& operator=(const UniqueIntArray&) = delete;

    UniqueIntArray(UniqueIntArray&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.data_ = nullptr;
        // TODO: 源对象的长度也应该清零。
    }

    UniqueIntArray& operator=(UniqueIntArray&& other) noexcept {
        // TODO: 释放旧缓冲区并接管 other 的资源。
        (void)other;
        return *this;
    }

    int size() const {
        return size_;
    }

    int* data() {
        return data_;
    }

    const int* data() const {
        return data_;
    }

    void set(int index, int value) {
        data_[index] = value;
    }

    int get(int index) const {
        return data_[index];
    }

private:
    int size_ = 0;
    int* data_ = nullptr;
};
