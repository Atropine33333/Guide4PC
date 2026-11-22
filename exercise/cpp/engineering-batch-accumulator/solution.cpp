// Reference solution: engineering-batch-accumulator
#include <cstddef>
#include <utility>
#include <vector>

class BatchAccumulator {
public:
    explicit BatchAccumulator(std::size_t batch_size)
        : batch_size_(batch_size == 0 ? 1 : batch_size), emitted_(0) {}

    bool add(int value) {
        if (pending_.size() >= batch_size_) return false;
        pending_.push_back(value);
        return true;
    }

    std::vector<int> take_batch() {
        if (pending_.size() != batch_size_) return {};
        std::vector<int> result;
        result.swap(pending_);
        emitted_ += result.size();
        return result;
    }

    std::vector<int> flush() {
        std::vector<int> result;
        result.swap(pending_);
        emitted_ += result.size();
        return result;
    }

    std::size_t pending() const { return pending_.size(); }
    std::size_t emitted() const { return emitted_; }

private:
    std::size_t batch_size_;
    std::size_t emitted_;
    std::vector<int> pending_;
};
