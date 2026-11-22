class LruCache {
public:
    explicit LruCache(int capacity) : capacity_(capacity) {}

    bool get(int key, int& out) {
        (void)key;
        (void)out;
        // TODO: 命中时刷新最近使用顺序。
        return false;
    }

    void put(int key, int value) {
        (void)key;
        (void)value;
        // TODO: 插入或覆盖；必要时淘汰最久未使用的键。
    }

    int size() const {
        // TODO: 返回键的数量。
        return 0;
    }

    int capacity() const { return capacity_; }

private:
    int capacity_ = 0;
};
