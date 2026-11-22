#include <vector>

class IntBST {
public:
    IntBST() = default;
    ~IntBST() = default;
    IntBST(const IntBST&) = delete;
    IntBST& operator=(const IntBST&) = delete;

    bool insert(int value) {
        // TODO: 拒绝重复值，并按 BST 规则插入。
        values_.push_back(value);
        return true;
    }

    bool contains(int value) const {
        for (int stored : values_) {
            if (stored == value) return true;
        }
        return false;
    }

    int size() const {
        return static_cast<int>(values_.size());
    }

    int height() const {
        // TODO: 计算真实的层数。
        return values_.empty() ? 0 : 1;
    }

    void inorder(std::vector<int>& out) const {
        // TODO: 中序遍历得到升序结果，而不是插入顺序。
        out.insert(out.end(), values_.begin(), values_.end());
    }

private:
    std::vector<int> values_;
};
