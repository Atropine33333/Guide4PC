// 参考实现：memory-deep-copy
#include <cstddef>
#include <utility>

class IntBuffer {
public:
    explicit IntBuffer(std::size_t size) : data_(size == 0 ? nullptr : new int[size]()), size_(size) {}

    IntBuffer(const IntBuffer& other) : data_(nullptr), size_(0) {
        if (other.size_ != 0) {
            data_ = new int[other.size_];
            size_ = other.size_;
            for (std::size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
        }
    }

    IntBuffer& operator=(const IntBuffer& other) {
        if (this == &other) return *this;
        IntBuffer copy(other);
        std::swap(data_, copy.data_);
        std::swap(size_, copy.size_);
        return *this;
    }

    ~IntBuffer() { delete[] data_; }

    std::size_t size() const { return size_; }

    int& at(std::size_t index) { return data_[index]; }
    const int& at(std::size_t index) const { return data_[index]; }

private:
    int* data_ = nullptr;
    std::size_t size_ = 0;
};
