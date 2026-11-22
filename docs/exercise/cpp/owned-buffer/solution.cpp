// 参考实现：owned-buffer
#include <cstddef>

class IntBuffer {
public:
    explicit IntBuffer(int size)
        : size_(size > 0 ? size : 0),
          data_(size_ > 0 ? new int[static_cast<std::size_t>(size_)]() : nullptr) {}

    IntBuffer(const IntBuffer& other) : size_(other.size_), data_(nullptr) {
        if (size_ > 0) {
            data_ = new int[static_cast<std::size_t>(size_)];
            for (int i = 0; i < size_; ++i) data_[i] = other.data_[i];
        }
    }

    IntBuffer& operator=(const IntBuffer& other) {
        if (this == &other) return *this;
        int *fresh = nullptr;
        if (other.size_ > 0) {
            fresh = new int[static_cast<std::size_t>(other.size_)];
            for (int i = 0; i < other.size_; ++i) fresh[i] = other.data_[i];
        }
        delete[] data_;
        data_ = fresh;
        size_ = other.size_;
        return *this;
    }

    ~IntBuffer() { delete[] data_; }

    int size() const { return size_; }

    bool set(int index, int value) {
        if (index < 0 || index >= size_) return false;
        data_[index] = value;
        return true;
    }

    bool get(int index, int& out) const {
        if (index < 0 || index >= size_) return false;
        out = data_[index];
        return true;
    }

    void fill(int value) {
        for (int i = 0; i < size_; ++i) data_[i] = value;
    }

private:
    int size_ = 0;
    int *data_ = nullptr;
};
