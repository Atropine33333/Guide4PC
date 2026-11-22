// Reference solution: oop-inventory
#include <cstddef>
#include <map>
#include <string>

class Inventory {
public:
    Inventory() = default;

    void add(const std::string& name, int quantity) {
        if (quantity <= 0) return;
        items_[name] += quantity;
    }

    bool remove(const std::string& name, int quantity) {
        if (quantity <= 0) return false;
        const auto it = items_.find(name);
        if (it == items_.end() || it->second < quantity) return false;
        it->second -= quantity;
        if (it->second == 0) items_.erase(it);
        return true;
    }

    int quantity(const std::string& name) const {
        const auto it = items_.find(name);
        return it == items_.end() ? 0 : it->second;
    }

    std::size_t distinct() const { return items_.size(); }

    long long total() const {
        long long sum = 0;
        for (const auto& entry : items_) sum += entry.second;
        return sum;
    }

private:
    std::map<std::string, int> items_;
};
