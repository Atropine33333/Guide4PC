// 参考实现：circular-buffer
#include <cstddef>
#include <vector>

class CircularBuffer {
public:
    explicit CircularBuffer(std::size_t capacity)
        : capacity_(capacity), head_(0), size_(0), data_(capacity) {}

    bool push(int value) {
        if (capacity_ == 0 || size_ == capacity_) return false;
        data_[(head_ + size_) % capacity_] = value;
        ++size_;
        return true;
    }

    bool pop(int &out) {
        if (size_ == 0) return false;
        out = data_[head_];
        head_ = (head_ + 1) % capacity_;
        --size_;
        return true;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }

private:
    std::size_t capacity_;
    std::size_t head_;
    std::size_t size_;
    std::vector<int> data_;
};
