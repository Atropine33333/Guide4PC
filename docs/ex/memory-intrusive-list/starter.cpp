#include <cstddef>

struct Node {
    int value;
    Node* prev;
    Node* next;
};

class IntrusiveList {
public:
    IntrusiveList() : head_(nullptr), tail_(nullptr), size_(0) {}
    ~IntrusiveList() {}

    void push_back(Node* node) {
        // TODO: 接到表尾，维护 prev/next/head/tail/size。
        (void)node;
    }

    void unlink(Node* node) {
        // TODO: 摘除结点并把它的链接置空。
        (void)node;
    }

    Node* head() const { return head_; }
    Node* tail() const { return tail_; }
    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

private:
    Node* head_;
    Node* tail_;
    std::size_t size_;
};
