// 参考实现：versioned-store
#include <map>
#include <vector>

class VersionedStore {
public:
    void put(int key, int value) {
        const auto it = data_.find(key);
        Change change;
        change.key = key;
        change.existed = it != data_.end();
        change.oldValue = change.existed ? it->second : 0;
        history_.push_back(change);
        data_[key] = value;
    }

    bool get(int key, int& out) const {
        const auto it = data_.find(key);
        if (it == data_.end()) return false;
        out = it->second;
        return true;
    }

    bool undo() {
        if (history_.empty()) return false;
        const Change change = history_.back();
        history_.pop_back();
        if (change.existed) {
            data_[change.key] = change.oldValue;
        } else {
            data_.erase(change.key);
        }
        return true;
    }

    int size() const { return static_cast<int>(data_.size()); }

    void clear() {
        data_.clear();
        history_.clear();
    }

private:
    struct Change {
        int key = 0;
        bool existed = false;
        int oldValue = 0;
    };
    std::map<int, int> data_;
    std::vector<Change> history_;
};
