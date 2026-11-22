// 参考实现：memory-weak-cycle
#include <memory>

struct Node {
    int value = 0;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> previous;
};

std::shared_ptr<Node> build_chain(int length) {
    if (length <= 0) return nullptr;

    auto head = std::make_shared<Node>();
    head->value = 0;
    std::shared_ptr<Node> tail = head;
    for (int i = 1; i < length; ++i) {
        auto node = std::make_shared<Node>();
        node->value = i;
        node->previous = tail;
        tail->next = node;
        tail = node;
    }
    return head;
}

std::size_t chain_length(const std::shared_ptr<Node>& head) {
    std::size_t count = 0;
    std::shared_ptr<Node> current = head;
    while (current != nullptr) {
        ++count;
        current = current->next;
    }
    return count;
}

bool links_consistent(const std::shared_ptr<Node>& head) {
    if (head == nullptr) return true;
    if (!head->previous.expired()) return false;

    std::shared_ptr<Node> current = head;
    while (current->next != nullptr) {
        const std::shared_ptr<Node> following = current->next;
        if (following->previous.lock().get() != current.get()) return false;
        current = following;
    }
    return true;
}
