// 参考实现：memory-move-owner
#include <utility>

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
        other.size_ = 0;
        other.data_ = nullptr;
    }

    UniqueIntArray& operator=(UniqueIntArray&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
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
