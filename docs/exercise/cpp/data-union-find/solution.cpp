// 参考实现：data-union-find
#include <vector>

class DisjointSet {
public:
    explicit DisjointSet(int n) : parent_(n), rank_(n, 0), count_(n) {
        for (int i = 0; i < n; ++i) parent_[i] = i;
    }

    int find(int x) {
        int root = x;
        while (parent_[root] != root) root = parent_[root];
        while (parent_[x] != x) {
            const int next = parent_[x];
            parent_[x] = root;
            x = next;
        }
        return root;
    }

    bool unite(int a, int b) {
        int root_a = find(a);
        int root_b = find(b);
        if (root_a == root_b) return false;
        if (rank_[root_a] < rank_[root_b]) {
            const int tmp = root_a;
            root_a = root_b;
            root_b = tmp;
        }
        parent_[root_b] = root_a;
        if (rank_[root_a] == rank_[root_b]) ++rank_[root_a];
        --count_;
        return true;
    }

    bool connected(int a, int b) {
        return find(a) == find(b);
    }

    int components() const {
        return count_;
    }

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
    int count_ = 0;
};
