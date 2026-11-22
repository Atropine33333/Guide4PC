// 参考实现：c-ring-buffer
#define RING_CAP 8

typedef struct { int data[RING_CAP]; int head; int size; } RingBuf;

void ring_init(RingBuf *r) {
    r->head = 0;
    r->size = 0;
}

int ring_push(RingBuf *r, int value) {
    if (r->size >= RING_CAP) return -1;
    r->data[(r->head + r->size) % RING_CAP] = value;
    ++r->size;
    return 0;
}

int ring_pop(RingBuf *r, int *out) {
    if (r->size == 0) return -1;
    *out = r->data[r->head];
    r->head = (r->head + 1) % RING_CAP;
    --r->size;
    return 0;
}

int ring_peek(const RingBuf *r, int *out) {
    if (r->size == 0) return -1;
    *out = r->data[r->head];
    return 0;
}

int ring_size(const RingBuf *r) {
    return r->size;
}
