// 参考实现：engineering-fixed-pool
#include <cstddef>

class IntPool {
public:
    explicit IntPool(int capacity);
    ~IntPool();

    IntPool(const IntPool&) = delete;
    IntPool& operator=(const IntPool&) = delete;

    int* acquire(int value);
    bool release(int* slot);
    int live() const;
    int capacity() const;

private:
    int* slots_;
    bool* used_;
    int capacity_;
    int live_;
};

IntPool::IntPool(int capacity)
    : slots_(nullptr), used_(nullptr), capacity_(capacity), live_(0) {
    if (capacity_ > 0) {
        slots_ = new int[static_cast<std::size_t>(capacity_)]();
        used_ = new bool[static_cast<std::size_t>(capacity_)]();
    }
}

IntPool::~IntPool() {
    delete[] slots_;
    delete[] used_;
}

int* IntPool::acquire(int value) {
    for (int i = 0; i < capacity_; ++i) {
        if (!used_[i]) {
            used_[i] = true;
            slots_[i] = value;
            ++live_;
            return &slots_[i];
        }
    }
    return nullptr;
}

bool IntPool::release(int* slot) {
    if (slot == nullptr) return false;
    for (int i = 0; i < capacity_; ++i) {
        if (slot == &slots_[i] && used_[i]) {
            used_[i] = false;
            --live_;
            return true;
        }
    }
    return false;
}

int IntPool::live() const {
    return live_;
}

int IntPool::capacity() const {
    return capacity_;
}
