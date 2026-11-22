#include <vector>

class OpenHashMap {
public:
    void put(int key, int value) {
        // TODO: 线性探测插入或更新。
        (void)key;
        (void)value;
    }

    bool get(int key, int& out) const {
        // TODO: 查找；失败时不要改 out。
        (void)key;
        (void)out;
        return false;
    }

    bool erase(int key) {
        // TODO: 删除并留下墓碑。
        (void)key;
        return false;
    }

    int size() const {
        return 0;  // TODO
    }
};
