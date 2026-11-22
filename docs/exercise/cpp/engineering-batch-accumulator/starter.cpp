#include <cstddef>
#include <vector>

class BatchAccumulator {
public:
    explicit BatchAccumulator(std::size_t batch_size)
        : batch_size_(batch_size), emitted_(0) {}

    bool add(int value) {
        // TODO: refuse to add when a full batch is already pending.
        pending_.push_back(value);
        return true;
    }

    std::vector<int> take_batch() {
        // TODO: only return a batch when it is exactly full.
        std::vector<int> result = pending_;
        pending_.clear();
        emitted_ += result.size();
        return result;
    }

    std::vector<int> flush() {
        // TODO: return the pending partial batch.
        return {};
    }

    std::size_t pending() const { return pending_.size(); }
    std::size_t emitted() const { return emitted_; }

private:
    std::size_t batch_size_;
    std::size_t emitted_;
    std::vector<int> pending_;
};
