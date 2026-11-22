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
    /* TODO: 借一节节点，填上 value，挂到表头前面。借不到就原样返回 head。 */
    (void)value;
    return head;
}

size_t list_length(const Node *head) {
    /* TODO */
    (void)head;
    return 0;
}

size_t list_to_array(const Node *head, int *out, size_t cap) {
    /* TODO: 从表头开始，把最多 cap 个值写进 out，返回实际写入个数。 */
    (void)head;
    (void)out;
    (void)cap;
    return 0;
}

void list_free_all(Node *head) {
    /* TODO: 从表头开始，把每一节都还给 node_free。 */
    (void)head;
}
