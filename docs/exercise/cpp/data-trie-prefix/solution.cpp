// 参考实现：data-trie-prefix
#include <string>
#include <vector>

class Trie {
public:
    Trie() : root_(new Node()) {}

    ~Trie() {
        destroy(root_);
    }

    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    void insert(const std::string& word) {
        Node* node = root_;
        ++node->passed;
        for (char c : word) {
            const int index = c - 'a';
            if (node->child[static_cast<std::size_t>(index)] == nullptr) {
                node->child[static_cast<std::size_t>(index)] = new Node();
            }
            node = node->child[static_cast<std::size_t>(index)];
            ++node->passed;
        }
        ++node->terminal;
        ++size_;
    }

    int count_prefix(const std::string& prefix) const {
        const Node* node = find(prefix);
        return node == nullptr ? 0 : node->passed;
    }

    int size() const {
        return size_;
    }

    bool contains(const std::string& word) const {
        const Node* node = find(word);
        return node != nullptr && node->terminal > 0;
    }

private:
    struct Node {
        Node* child[26] = {};
        int passed = 0;
        int terminal = 0;
    };

    const Node* find(const std::string& text) const {
        const Node* node = root_;
        for (char c : text) {
            node = node->child[static_cast<std::size_t>(c - 'a')];
            if (node == nullptr) return nullptr;
        }
        return node;
    }

    static void destroy(Node* node) {
        if (node == nullptr) return;
        for (Node*& child : node->child) {
            destroy(child);
        }
        delete node;
    }

    Node* root_ = nullptr;
    int size_ = 0;
};
