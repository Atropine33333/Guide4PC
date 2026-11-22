// 参考实现：data-bst-height
#include <vector>

class IntBST {
public:
    IntBST() = default;

    ~IntBST() {
        destroy(root_);
    }

    IntBST(const IntBST&) = delete;
    IntBST& operator=(const IntBST&) = delete;

    bool insert(int value) {
        return insert_node(root_, value);
    }

    bool contains(int value) const {
        const Node* node = root_;
        while (node != nullptr) {
            if (value == node->value) return true;
            node = value < node->value ? node->left : node->right;
        }
        return false;
    }

    int size() const {
        return size_;
    }

    int height() const {
        return height_node(root_);
    }

    void inorder(std::vector<int>& out) const {
        inorder_node(root_, out);
    }

private:
    struct Node {
        int value;
        Node* left;
        Node* right;

        explicit Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    bool insert_node(Node*& node, int value) {
        if (node == nullptr) {
            node = new Node(value);
            ++size_;
            return true;
        }
        if (value == node->value) return false;
        return value < node->value ? insert_node(node->left, value)
                                   : insert_node(node->right, value);
    }

    static int height_node(const Node* node) {
        if (node == nullptr) return 0;
        const int left = height_node(node->left);
        const int right = height_node(node->right);
        return 1 + (left > right ? left : right);
    }

    static void inorder_node(const Node* node, std::vector<int>& out) {
        if (node == nullptr) return;
        inorder_node(node->left, out);
        out.push_back(node->value);
        inorder_node(node->right, out);
    }

    static void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    Node* root_ = nullptr;
    int size_ = 0;
};
