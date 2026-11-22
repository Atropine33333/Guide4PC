// Reference solution: engineering-idempotent-gateway
#include <cstddef>
#include <map>
#include <optional>

enum class SubmitStatus { accepted, duplicate, conflict };

class Gateway {
public:
    Gateway() = default;

    SubmitStatus submit(int id, int amount) {
        const auto it = records_.find(id);
        if (it == records_.end()) {
            Record record;
            record.amount = amount;
            records_.emplace(id, record);
            return SubmitStatus::accepted;
        }
        return it->second.amount == amount ? SubmitStatus::duplicate
                                           : SubmitStatus::conflict;
    }

    bool complete(int id, int result) {
        const auto it = records_.find(id);
        if (it == records_.end() || it->second.completed) return false;
        it->second.completed = true;
        it->second.result = result;
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
        for (const auto& entry : records_) {
            if (!entry.second.completed) ++count;
        }
        return count;
    }

    std::size_t completed() const {
        std::size_t count = 0;
        for (const auto& entry : records_) {
            if (entry.second.completed) ++count;
        }
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
