// Reference solution: memory-generational-handle
#include <cstddef>
#include <cstdint>
#include <vector>

struct Handle {
    std::uint32_t index;
    std::uint32_t generation;
};

class HandleStore {
public:
    static constexpr std::uint32_t kCapacity = 8;
    static constexpr std::uint32_t kInvalidIndex = 0xffffffffu;

    HandleStore() : size_(0) {
        for (std::uint32_t i = kCapacity; i > 0; --i) {
            free_.push_back(i - 1);
        }
    }

    Handle create(int value) {
        if (free_.empty()) return {kInvalidIndex, 0};
        const std::uint32_t index = free_.back();
        free_.pop_back();
        Slot& slot = slots_[index];
        slot.value = value;
        slot.occupied = true;
        ++size_;
        return {index, slot.generation};
    }

    bool valid(Handle handle) const {
        if (handle.index >= kCapacity) return false;
        const Slot& slot = slots_[handle.index];
        return slot.occupied && slot.generation == handle.generation;
    }

    int* get(Handle handle) {
        if (!valid(handle)) return nullptr;
        return &slots_[handle.index].value;
    }

    const int* get(Handle handle) const {
        if (!valid(handle)) return nullptr;
        return &slots_[handle.index].value;
    }

    bool destroy(Handle handle) {
        if (!valid(handle)) return false;
        Slot& slot = slots_[handle.index];
        slot.occupied = false;
        ++slot.generation;
        if (slot.generation == 0) slot.generation = 1;
        --size_;
        free_.push_back(handle.index);
        return true;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return kCapacity; }

private:
    struct Slot {
        int value = 0;
        std::uint32_t generation = 1;
        bool occupied = false;
    };

    Slot slots_[kCapacity];
    std::vector<std::uint32_t> free_;
    std::size_t size_;
};
