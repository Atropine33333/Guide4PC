#include <cstddef>

class IntList {
public:
    IntList() = default;
    ~IntList() = default;

    void pushFront(int value) {
        (void)value;
        // TODO: 在头部插入结点。
    }

    bool popFront() {
        // TODO: 删除头结点。
        return false;
    }

    void reverse() {
        // TODO: 就地反转链表。
    }

    bool get(int index, int& out) const {
        (void)index;
        (void)out;
        // TODO: 按 0 开始的下标取值。
        return false;
    }

    int size() const {
        // TODO: 返回元素个数。
        return 0;
    }

private:
    struct Node {
        int value = 0;
        Node *next = nullptr;
    };
    Node *head_ = nullptr;
    int size_ = 0;
};
