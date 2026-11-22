// 参考实现：engineering-arena-allocator
#include <cstddef>

class Arena {
public:
    explicit Arena(int capacity) : capacity_(capacity) {}

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    void* allocate(int size, int alignment) {
        if (size <= 0) return nullptr;
        const int aligned = (used_ + alignment - 1) / alignment * alignment;
        if (aligned + size > capacity_) return nullptr;
        void* result = buffer_ + aligned;
        used_ = aligned + size;
        return result;
    }

    int used() const {
        return used_;
    }

    int capacity() const {
        return capacity_;
    }

    void reset() {
        used_ = 0;
    }

private:
    alignas(16) unsigned char buffer_[256]{};
    int capacity_ = 0;
    int used_ = 0;
};
