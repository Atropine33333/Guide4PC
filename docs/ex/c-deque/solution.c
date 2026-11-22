// 参考实现：c-deque
#include <stddef.h>

#define DEQUE_CAP 64

typedef struct {
    int data[DEQUE_CAP];
    int head;
    int size;
} Deque;

void deque_init(Deque *d) {
    d->head = 0;
    d->size = 0;
}

int deque_push_front(Deque *d, int value) {
    if (d->size >= DEQUE_CAP) return -1;
    d->head = (d->head + DEQUE_CAP - 1) % DEQUE_CAP;
    d->data[d->head] = value;
    ++d->size;
    return 0;
}

int deque_push_back(Deque *d, int value) {
    if (d->size >= DEQUE_CAP) return -1;
    d->data[(d->head + d->size) % DEQUE_CAP] = value;
    ++d->size;
    return 0;
}

int deque_pop_front(Deque *d, int *out) {
    if (d->size == 0) return -1;
    *out = d->data[d->head];
    d->head = (d->head + 1) % DEQUE_CAP;
    --d->size;
    return 0;
}

int deque_pop_back(Deque *d, int *out) {
    if (d->size == 0) return -1;
    *out = d->data[(d->head + d->size - 1) % DEQUE_CAP];
    --d->size;
    return 0;
}

int deque_size(const Deque *d) {
    return d->size;
}
