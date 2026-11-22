#include <cstddef>
#include <map>
#include <optional>

enum class SubmitStatus { accepted, duplicate, conflict };

class Gateway {
public:
    Gateway() = default;

    SubmitStatus submit(int id, int amount) {
        // TODO: detect duplicates and conflicts.
        records_[id].amount = amount;
        return SubmitStatus::accepted;
    }

    bool complete(int id, int result) {
        // TODO: reject unknown and already completed ids.
        records_[id].completed = true;
        records_[id].result = result;
        return true;
    }

    bool is_completed(int id) const {
        const auto it = records_.find(id);
        return it != records_.end() && it->second.completed;
    }

    std::optional<int> result_of(int id) const {
        const auto it = records_.find(id);
        if (it == records_.end() || !it->second.completed) return std::nullopt;
        return it->second.result;
    }

    std::size_t pending() const {
        std::size_t count = 0;
        for (const auto& entry : records_) count += entry.second.completed ? 0 : 1;
        return count;
    }

    std::size_t completed() const {
        std::size_t count = 0;
        for (const auto& entry : records_) count += entry.second.completed ? 1 : 0;
        return count;
    }

private:
    struct Record {
        int amount = 0;
        bool completed = false;
        int result = 0;
    };

    std::map<int, Record> records_;
};
