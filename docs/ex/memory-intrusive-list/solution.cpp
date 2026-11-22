#include <cstddef>

struct Node {
    int value;
    Node* prev;
    Node* next;
};

class IntrusiveList {
public:
    IntrusiveList() : head_(nullptr), tail_(nullptr), size_(0) {}
    ~IntrusiveList() = default;

    void push_back(Node* node) {
        node->prev = tail_;
        node->next = nullptr;
        if (tail_ != nullptr) {
            tail_->next = node;
        } else {
            head_ = node;
        }
        tail_ = node;
        ++size_;
    }

    void unlink(Node* node) {
        if (node == nullptr || !contains(node)) {
            return;
        }
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            head_ = node->next;
        }
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            tail_ = node->prev;
        }
        node->prev = nullptr;
        node->next = nullptr;
        --size_;
    }

    Node* head() const { return head_; }
    Node* tail() const { return tail_; }
    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

private:
    bool contains(const Node* node) const {
        for (const Node* current = head_; current != nullptr;
             current = current->next) {
            if (current == node) {
                return true;
            }
        }
        return false;
    }

    Node* head_;
    Node* tail_;
    std::size_t size_;
};
