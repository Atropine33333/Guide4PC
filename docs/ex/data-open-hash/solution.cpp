#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

class OpenHashMap {
public:
    OpenHashMap() : table_(8), used_(0), tombstones_(0) {}

    void put(int key, int value) {
        if ((used_ + tombstones_ + 1) * 10 >=
            static_cast<int>(table_.size()) * 7) {
            rehash(table_.size() * 2);
        }
        insert(key, value);
        ++used_;
    }

    bool get(int key, int& out) const {
        const std::size_t mask = table_.size();
        std::size_t index = bucket(key);
        for (std::size_t probe = 0; probe < mask; ++probe) {
            const Entry& entry = table_[index];
            if (entry.state == kEmpty) {
                return false;
            }
            if (entry.state == kUsed && entry.key == key) {
                out = entry.value;
                return true;
            }
            index = (index + 1) % mask;
        }
        return false;
    }

    bool erase(int key) {
        const std::size_t mask = table_.size();
        std::size_t index = bucket(key);
        for (std::size_t probe = 0; probe < mask; ++probe) {
            Entry& entry = table_[index];
            if (entry.state == kEmpty) {
                return false;
            }
            if (entry.state == kUsed && entry.key == key) {
                entry.state = kDeleted;
                --used_;
                ++tombstones_;
                return true;
            }
            index = (index + 1) % mask;
        }
        return false;
    }

    int size() const { return static_cast<int>(used_); }

private:
    enum : unsigned char { kEmpty = 0, kUsed = 1, kDeleted = 2 };

    struct Entry {
        int key = 0;
        int value = 0;
        unsigned char state = kEmpty;
    };

    std::size_t bucket(int key) const {
        return std::hash<int>{}(key) % table_.size();
    }

    // Called only when the key is known to be absent; claims a fresh slot.
    void insert(int key, int value) {
        const std::size_t mask = table_.size();
        std::size_t index = bucket(key);
        std::size_t first_deleted = mask;
        for (std::size_t probe = 0; probe < mask; ++probe) {
            Entry& entry = table_[index];
            if (entry.state == kUsed) {
                if (entry.key == key) {
                    entry.value = value;
                    --used_;
                    return;
                }
            } else if (entry.state == kDeleted) {
                if (first_deleted == mask) {
                    first_deleted = index;
                }
            } else {
                const std::size_t target =
                    first_deleted == mask ? index : first_deleted;
                Entry& slot = table_[target];
                if (slot.state == kDeleted) {
                    --tombstones_;
                }
                slot.key = key;
                slot.value = value;
                slot.state = kUsed;
                return;
            }
            index = (index + 1) % mask;
        }
        if (first_deleted != mask) {
            Entry& slot = table_[first_deleted];
            slot.key = key;
            slot.value = value;
            slot.state = kUsed;
            --tombstones_;
            return;
        }
        rehash(table_.size() * 2);
        insert(key, value);
    }

    void rehash(std::size_t capacity) {
        std::vector<Entry> old;
        old.swap(table_);
        table_.assign(capacity, Entry{});
        used_ = 0;
        tombstones_ = 0;
        for (const Entry& entry : old) {
            if (entry.state == kUsed) {
                insert(entry.key, entry.value);
                ++used_;
            }
        }
    }

    std::vector<Entry> table_;
    int used_;
    int tombstones_;
};
