#include <stddef.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *list_reverse(Node *head);

Node *list_reverse(Node *head) {
    /* TODO: 把链表就地反转，返回新的头节点。 */
    (void)head;
    return NULL;
}
