// 参考实现：c-queue
#include <stddef.h>

#define QUEUE_CAP 8

typedef struct {
    int data[QUEUE_CAP];
    size_t head;
    size_t len;
} IntQueue;

void queue_init(IntQueue *q) {
    q->head = 0;
    q->len = 0;
}

int queue_push(IntQueue *q, int value) {
    if (q->len >= QUEUE_CAP) return -1;
    size_t tail = (q->head + q->len) % QUEUE_CAP;
    q->data[tail] = value;
    q->len += 1;
    return 0;
}

int queue_pop(IntQueue *q, int *out) {
    if (q->len == 0) return -1;
    *out = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_CAP;
    q->len -= 1;
    return 0;
}

size_t queue_size(const IntQueue *q) {
    return q->len;
}

int queue_is_empty(const IntQueue *q) {
    return q->len == 0 ? 1 : 0;
}

int queue_is_full(const IntQueue *q) {
    return q->len == QUEUE_CAP ? 1 : 0;
}
