// 参考实现：data-linked-reverse
#include <cstddef>

struct Node {
    int value;
    Node* next;
};

class IntList {
public:
    IntList();
    ~IntList();
    IntList(const IntList&) = delete;
    IntList& operator=(const IntList&) = delete;

    void push_front(int value);
    void reverse();
    int size() const;
    const Node* head() const;

private:
    Node* head_;
    int size_;
};

IntList::IntList() : head_(nullptr), size_(0) {}

IntList::~IntList() {
    Node* current = head_;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void IntList::push_front(int value) {
    head_ = new Node{value, head_};
    ++size_;
}

void IntList::reverse() {
    Node* previous = nullptr;
    Node* current = head_;
    while (current != nullptr) {
        Node* next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    head_ = previous;
}

int IntList::size() const {
    return size_;
}

const Node* IntList::head() const {
    return head_;
}
