#include <stddef.h>

#define DEQUE_CAP 64

typedef struct {
    int data[DEQUE_CAP];
    int head;
    int size;
} Deque;

void deque_init(Deque *d);
int deque_push_front(Deque *d, int value);
int deque_push_back(Deque *d, int value);
int deque_pop_front(Deque *d, int *out);
int deque_pop_back(Deque *d, int *out);
int deque_size(const Deque *d);

void deque_init(Deque *d) {
    /* TODO: 置空。 */
    (void)d;
}

int deque_push_front(Deque *d, int value) {
    /* TODO: 满了返回 -1，否则放到前端并返回 0。 */
    (void)d;
    (void)value;
    return -1;
}

int deque_push_back(Deque *d, int value) {
    /* TODO: 满了返回 -1，否则放到后端并返回 0。 */
    (void)d;
    (void)value;
    return -1;
}

int deque_pop_front(Deque *d, int *out) {
    /* TODO: 空了返回 -1，否则把前端写进 out 并返回 0。 */
    (void)d;
    (void)out;
    return -1;
}

int deque_pop_back(Deque *d, int *out) {
    /* TODO: 空了返回 -1，否则把后端写进 out 并返回 0。 */
    (void)d;
    (void)out;
    return -1;
}

int deque_size(const Deque *d) {
    /* TODO: 返回元素个数。 */
    (void)d;
    return 0;
}
