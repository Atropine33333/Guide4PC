#include <stddef.h>

#define QUEUE_CAP 8

typedef struct {
    int data[QUEUE_CAP];
    size_t head;
    size_t len;
} IntQueue;

void queue_init(IntQueue *q);
int queue_push(IntQueue *q, int value);
int queue_pop(IntQueue *q, int *out);
size_t queue_size(const IntQueue *q);
int queue_is_empty(const IntQueue *q);
int queue_is_full(const IntQueue *q);

void queue_init(IntQueue *q) {
    /* TODO: 队头和长度归零。 */
    q->head = 0;
    q->len = 0;
}

int queue_push(IntQueue *q, int value) {
    /* TODO: 满了返回 -1；否则写到队尾并返回 0。 */
    (void)q;
    (void)value;
    return -1;
}

int queue_pop(IntQueue *q, int *out) {
    /* TODO: 空了返回 -1；否则把队头写进 *out 并返回 0。 */
    (void)q;
    (void)out;
    return -1;
}

size_t queue_size(const IntQueue *q) {
    /* TODO */
    (void)q;
    return 0;
}

int queue_is_empty(const IntQueue *q) {
    /* TODO */
    (void)q;
    return 1;
}

int queue_is_full(const IntQueue *q) {
    /* TODO */
    (void)q;
    return 0;
}
