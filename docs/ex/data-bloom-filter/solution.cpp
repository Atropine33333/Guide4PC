// Reference solution: data-bloom-filter
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class BloomFilter {
public:
    explicit BloomFilter(std::size_t bits, std::size_t hash_count)
        : bits_(bits == 0 ? 1 : bits), hashes_(hash_count == 0 ? 1 : hash_count),
          storage_(bits_, 0) {}

    void add(const std::string& value) {
        std::uint64_t first = 1469598103934665603ULL;
        std::uint64_t second = 1099511628211ULL;
        for (unsigned char ch : value) {
            first = (first ^ ch) * 1099511628211ULL;
            second = (second + ch + 0x9e3779b97f4a7c15ULL) * 0x100000001b3ULL;
        }
        for (std::size_t i = 0; i < hashes_; ++i) {
            const std::size_t index = static_cast<std::size_t>(
                (first + i * second) % bits_);
            storage_[index] = 1;
        }
    }

    bool maybe_contains(const std::string& value) const {
        std::uint64_t first = 1469598103934665603ULL;
        std::uint64_t second = 1099511628211ULL;
        for (unsigned char ch : value) {
            first = (first ^ ch) * 1099511628211ULL;
            second = (second + ch + 0x9e3779b97f4a7c15ULL) * 0x100000001b3ULL;
        }
        for (std::size_t i = 0; i < hashes_; ++i) {
            const std::size_t index = static_cast<std::size_t>(
                (first + i * second) % bits_);
            if (storage_[index] == 0) return false;
        }
        return true;
    }

    std::size_t set_bits() const {
        std::size_t count = 0;
        for (unsigned char bit : storage_) count += bit == 0 ? 0 : 1;
        return count;
    }

    std::size_t bit_count() const { return bits_; }

private:
    std::size_t bits_;
    std::size_t hashes_;
    std::vector<unsigned char> storage_;
};
