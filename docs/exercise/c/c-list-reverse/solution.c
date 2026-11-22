// 参考实现：c-list-reverse
#include <stddef.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *list_reverse(Node *head) {
    Node *prev = NULL;
    while (head != NULL) {
        Node *next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    return prev;
}
