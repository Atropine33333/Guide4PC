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
    // TODO: 释放所有结点。
}

void IntList::push_front(int value) {
    (void)value;
    // TODO: 新建结点，接到表头。
}

void IntList::reverse() {
    // TODO: 原地反转，不要交换值。
}

int IntList::size() const {
    return size_;
}

const Node* IntList::head() const {
    return head_;
}
