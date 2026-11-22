#include <stddef.h>

#define INT_STACK_CAP 128

typedef struct {
    int data[INT_STACK_CAP];
    size_t size;
} IntStack;

void stack_init(IntStack *s);
int stack_push(IntStack *s, int value);
int stack_pop(IntStack *s, int *out);
int stack_peek(const IntStack *s, int *out);
int stack_size(const IntStack *s);

void stack_init(IntStack *s) {
    /* TODO: 置空。 */
    (void)s;
}

int stack_push(IntStack *s, int value) {
    /* TODO: 满了返回 -1，否则压入并返回 0。 */
    (void)s;
    (void)value;
    return -1;
}

int stack_pop(IntStack *s, int *out) {
    /* TODO: 空了返回 -1，否则弹出到 out 并返回 0。 */
    (void)s;
    (void)out;
    return -1;
}

int stack_peek(const IntStack *s, int *out) {
    /* TODO: 只看栈顶，不弹出。 */
    (void)s;
    (void)out;
    return -1;
}

int stack_size(const IntStack *s) {
    /* TODO */
    (void)s;
    return 0;
}
