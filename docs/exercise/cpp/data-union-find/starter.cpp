class DisjointSet {
public:
    explicit DisjointSet(int n) : count_(n) {}

    int find(int x) {
        // TODO: 找到代表元，并做路径压缩。
        return x;
    }

    bool unite(int a, int b) {
        // TODO: 合并两个不同的组。
        (void)a;
        (void)b;
        return false;
    }

    bool connected(int a, int b) {
        return find(a) == find(b);
    }

    int components() const {
        return count_;
    }

private:
    int count_ = 0;
};
