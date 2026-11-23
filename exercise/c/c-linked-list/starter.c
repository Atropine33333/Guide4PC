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

void list_init(IntList *l);
int list_push_front(IntList *l, int value);
int list_pop_front(IntList *l, int *out);
size_t list_size(const IntList *l);
int list_is_empty(const IntList *l);
void list_free(IntList *l);

void list_init(IntList *l) {
    /* TODO: 头指针置空，长度归零。 */
    l->head = NULL;
    l->size = 0;
}

int list_push_front(IntList *l, int value) {
    /* TODO: 造一个节点挂到最前面，成功返回 0。 */
    (void)l;
    (void)value;
    return -1;
}

int list_pop_front(IntList *l, int *out) {
    /* TODO: 摘掉头节点，值写进 *out，然后释放它。 */
    (void)l;
    (void)out;
    return -1;
}

size_t list_size(const IntList *l) {
    /* TODO */
    (void)l;
    return 0;
}

int list_is_empty(const IntList *l) {
    /* TODO: 空表返回 1。 */
    (void)l;
    return 1;
}

void list_free(IntList *l) {
    /* TODO: 释放全部节点，并把 head 与 size 复位。 */
    (void)l;
}
