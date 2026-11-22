#include <cstddef>

class Arena {
public:
    explicit Arena(int capacity) : capacity_(capacity) {}

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    void* allocate(int size, int alignment) {
        if (size <= 0) return nullptr;
        if (used_ + size > capacity_) return nullptr;
        // TODO: 计算对齐填充，保证返回地址满足 alignment。
        (void)alignment;
        void* result = buffer_ + used_;
        used_ += size;
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
