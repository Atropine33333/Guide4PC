// 参考实现：c-linked-list
#include <stddef.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t size;
} IntList;

void list_init(IntList *l) {
    l->head = NULL;
    l->size = 0;
}

int list_push_front(IntList *l, int value) {
    Node *node = (Node *)malloc(sizeof *node);
    if (node == NULL) return -1;
    node->value = value;
    node->next = l->head;
    l->head = node;
    l->size += 1;
    return 0;
}

int list_pop_front(IntList *l, int *out) {
    if (l->head == NULL) return -1;
    Node *node = l->head;
    *out = node->value;
    l->head = node->next;
    l->size -= 1;
    free(node);
    return 0;
}

size_t list_size(const IntList *l) {
    return l->size;
}

int list_is_empty(const IntList *l) {
    return l->head == NULL ? 1 : 0;
}

void list_free(IntList *l) {
    Node *node = l->head;
    while (node != NULL) {
        Node *next = node->next;
        free(node);
        node = next;
    }
    l->head = NULL;
    l->size = 0;
}
