#include <map>
#include <vector>

class VersionedStore {
public:
    void put(int key, int value) {
        (void)key;
        (void)value;
        // TODO: 写入键值，并记录足够的信息供 undo 使用。
    }

    bool get(int key, int& out) const {
        (void)key;
        (void)out;
        // TODO: 查找键。
        return false;
    }

    bool undo() {
        // TODO: 撤销最近一次 put。
        return false;
    }

    int size() const {
        // TODO: 返回键的数量。
        return 0;
    }

    void clear() {
        // TODO: 清空键与撤销历史。
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
