#include <cstddef>

class FixedQueue {
public:
    explicit FixedQueue(int capacity);
    ~FixedQueue();

    FixedQueue(const FixedQueue&) = delete;
    FixedQueue& operator=(const FixedQueue&) = delete;

    bool push(int value);
    bool pop(int& value);
    int size() const;
    int capacity() const;
    void clear();

private:
    int* data_;
    int capacity_;
    int head_;
    int count_;
};

FixedQueue::FixedQueue(int capacity)
    : data_(nullptr), capacity_(capacity), head_(0), count_(0) {
    if (capacity_ > 0) data_ = new int[static_cast<std::size_t>(capacity_)];
}

FixedQueue::~FixedQueue() {
    delete[] data_;
}

bool FixedQueue::push(int value) {
    if (capacity_ == 0) return false;
    if (count_ == capacity_) {
        // BUG: 满队列时覆盖最旧的元素，并错误地报告成功。
        head_ = (head_ + 1) % capacity_;
        --count_;
    }
    data_[(head_ + count_) % capacity_] = value;
    ++count_;
    return true;
}

bool FixedQueue::pop(int& value) {
    if (count_ == 0) return false;
    value = data_[head_];
    head_ = (head_ + 1) % capacity_;
    --count_;
    return true;
}

int FixedQueue::size() const {
    return count_;
}

int FixedQueue::capacity() const {
    return capacity_;
}

void FixedQueue::clear() {
    head_ = 0;
    count_ = 0;
}
