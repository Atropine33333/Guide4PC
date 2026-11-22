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
        tail->next = node;
        tail = node;
    }
    // TODO: 还没有设置 previous 弱引用。
    return head;
}

std::size_t chain_length(const std::shared_ptr<Node>& head) {
    (void)head;
    // TODO: 还没有沿 next 计数。
    return 0;
}

bool links_consistent(const std::shared_ptr<Node>& head) {
    (void)head;
    // TODO: 还没有检查链接。
    return true;
}
