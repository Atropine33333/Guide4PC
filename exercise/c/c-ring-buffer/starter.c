#define RING_CAP 8

typedef struct { int data[RING_CAP]; int head; int size; } RingBuf;

void ring_init(RingBuf *r);
int ring_push(RingBuf *r, int value);
int ring_pop(RingBuf *r, int *out);
int ring_peek(const RingBuf *r, int *out);
int ring_size(const RingBuf *r);

void ring_init(RingBuf *r) {
    /* TODO: 置空。 */
    (void)r;
}

int ring_push(RingBuf *r, int value) {
    /* TODO: 满了返回 -1，否则写到队尾并返回 0。 */
    (void)r;
    (void)value;
    return -1;
}

int ring_pop(RingBuf *r, int *out) {
    /* TODO: 空了返回 -1，否则把队头写进 out 并返回 0。 */
    (void)r;
    (void)out;
    return -1;
}

int ring_peek(const RingBuf *r, int *out) {
    /* TODO: 只看队头，不取走。 */
    (void)r;
    (void)out;
    return -1;
}

int ring_size(const RingBuf *r) {
    /* TODO */
    (void)r;
    return 0;
}
