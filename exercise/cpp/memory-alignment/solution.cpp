// 参考实现：memory-alignment
#include <cstddef>
#include <cstdint>
#include <limits>

namespace {

bool is_power_of_two(std::size_t value) {
    return value != 0 && (value & (value - 1)) == 0;
}

}  // namespace

std::size_t align_up(std::size_t value, std::size_t alignment) {
    if (alignment == 0) return value;
    const std::size_t mask = alignment - 1;
    if (value > std::numeric_limits<std::size_t>::max() - mask) {
        return std::numeric_limits<std::size_t>::max();
    }
    return (value + mask) & ~mask;
}

bool is_aligned(const void* pointer, std::size_t alignment) {
    if (!is_power_of_two(alignment)) return false;
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(pointer);
    return address % alignment == 0;
}

std::size_t padding_for(std::size_t used, std::size_t alignment) {
    if (alignment == 0) return 0;
    return (alignment - used % alignment) % alignment;
}
