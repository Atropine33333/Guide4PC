// 参考实现：lru-cache
#include <list>
#include <unordered_map>
#include <utility>

class LruCache {
public:
    explicit LruCache(int capacity) : capacity_(capacity) {}

    bool get(int key, int& out) {
        const auto it = positions_.find(key);
        if (it == positions_.end()) return false;
        out = it->second->second;
        entries_.splice(entries_.begin(), entries_, it->second);
        return true;
    }

    void put(int key, int value) {
        if (capacity_ <= 0) return;
        const auto it = positions_.find(key);
        if (it != positions_.end()) {
            it->second->second = value;
            entries_.splice(entries_.begin(), entries_, it->second);
            return;
        }
        if (static_cast<int>(entries_.size()) >= capacity_) {
            positions_.erase(entries_.back().first);
            entries_.pop_back();
        }
        entries_.emplace_front(key, value);
        positions_[key] = entries_.begin();
    }

    int size() const { return static_cast<int>(entries_.size()); }

    int capacity() const { return capacity_; }

private:
    int capacity_ = 0;
    std::list<std::pair<int, int>> entries_;
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> positions_;
};
