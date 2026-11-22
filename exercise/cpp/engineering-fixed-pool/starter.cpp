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
    if (capacity_ <= 0 || slots_ == nullptr) return nullptr;
    // TODO: 找到第一个空闲槽位并标记使用。
    // 暂时的占位实现总是复用第一个槽位。
    *slots_ = value;
    return slots_;
}

bool IntPool::release(int* slot) {
    (void)slot;
    // TODO: 只接受池内当前已使用的槽位。
    return false;
}

int IntPool::live() const {
    return live_;
}

int IntPool::capacity() const {
    return capacity_;
}
