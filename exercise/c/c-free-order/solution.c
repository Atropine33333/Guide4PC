// 参考实现：c-free-order
#include <stddef.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

/* 这两个函数由题目提供：借一节节点、归还一节节点。 */
Node *node_alloc(void);
void node_free(Node *p);

Node *list_push_front(Node *head, int value);
size_t list_length(const Node *head);
size_t list_to_array(const Node *head, int *out, size_t cap);
void list_free_all(Node *head);

Node *list_push_front(Node *head, int value) {
    Node *fresh = node_alloc();
    if (fresh == NULL) return head;
    fresh->value = value;
    fresh->next = head;
    return fresh;
}

size_t list_length(const Node *head) {
    size_t n = 0;
    while (head != NULL) {
        ++n;
        head = head->next;
    }
    return n;
}

size_t list_to_array(const Node *head, int *out, size_t cap) {
    size_t n = 0;
    while (head != NULL && n < cap) {
        out[n++] = head->value;
        head = head->next;
    }
    return n;
}

void list_free_all(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        node_free(head);
        head = next;
    }
}
