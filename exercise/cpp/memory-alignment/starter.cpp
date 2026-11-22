#include <cstddef>

std::size_t align_up(std::size_t value, std::size_t alignment) {
    (void)alignment;
    // TODO: 还没有向上对齐。
    return value;
}

bool is_aligned(const void* pointer, std::size_t alignment) {
    (void)alignment;
    // TODO: 只判断了非空。
    return pointer != nullptr;
}

std::size_t padding_for(std::size_t used, std::size_t alignment) {
    (void)used;
    (void)alignment;
    // TODO: 还没有计算补齐字节数。
    return 0;
}
