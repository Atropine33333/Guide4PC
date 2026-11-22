// 参考实现：int-list-ops
#include <cstddef>

class IntList {
public:
    IntList() = default;

    ~IntList() {
        while (head_ != nullptr) {
            Node *next = head_->next;
            delete head_;
            head_ = next;
        }
    }

    void pushFront(int value) {
        head_ = new Node{value, head_};
        ++size_;
    }

    bool popFront() {
        if (head_ == nullptr) return false;
        Node *next = head_->next;
        delete head_;
        head_ = next;
        --size_;
        return true;
    }

    void reverse() {
        Node *previous = nullptr;
        Node *current = head_;
        while (current != nullptr) {
            Node *next = current->next;
            current->next = previous;
            previous = current;
            current = next;
        }
        head_ = previous;
    }

    bool get(int index, int& out) const {
        if (index < 0 || index >= size_) return false;
        const Node *current = head_;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        out = current->value;
        return true;
    }

    int size() const { return size_; }

private:
    struct Node {
        int value = 0;
        Node *next = nullptr;
    };
    Node *head_ = nullptr;
    int size_ = 0;
};
