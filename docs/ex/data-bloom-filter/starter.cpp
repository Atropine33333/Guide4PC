#include <cstddef>
#include <string>
#include <vector>

class BloomFilter {
public:
    explicit BloomFilter(std::size_t bits, std::size_t hash_count)
        : bits_(bits == 0 ? 1 : bits), hashes_(hash_count == 0 ? 1 : hash_count),
          storage_(bits_, 0) {}

    void add(const std::string& value) {
        // TODO: set the probe bits for value.
        (void)value;
    }

    bool maybe_contains(const std::string& value) const {
        // TODO: inspect the probe bits instead of always returning false.
        (void)value;
        return false;
    }

    std::size_t set_bits() const {
        // TODO: count the set bits.
        return 0;
    }

    std::size_t bit_count() const { return bits_; }

private:
    std::size_t bits_;
    std::size_t hashes_;
    std::vector<unsigned char> storage_;
};
