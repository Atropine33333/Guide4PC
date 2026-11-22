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

    HandleStore() = default;

    Handle create(int value) {
        // TODO: reuse freed slots and bump generations.
        if (values_.size() >= kCapacity) return {kInvalidIndex, 0};
        values_.push_back(value);
        return {static_cast<std::uint32_t>(values_.size() - 1), 1};
    }

    bool valid(Handle handle) const {
        return handle.index < values_.size() && handle.generation == 1;
    }

    int* get(Handle handle) {
        if (!valid(handle)) return nullptr;
        return &values_[handle.index];
    }

    const int* get(Handle handle) const {
        if (!valid(handle)) return nullptr;
        return &values_[handle.index];
    }

    bool destroy(Handle handle) {
        // TODO: invalidate the old generation.
        if (!valid(handle)) return false;
        values_[handle.index] = 0;
        return true;
    }

    std::size_t size() const { return values_.size(); }
    std::size_t capacity() const { return kCapacity; }

private:
    std::vector<int> values_;
};
