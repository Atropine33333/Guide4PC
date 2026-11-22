// 参考实现：c-int-stack
#include <stddef.h>

#define INT_STACK_CAP 128

typedef struct {
    int data[INT_STACK_CAP];
    size_t size;
} IntStack;

void stack_init(IntStack *s) {
    s->size = 0;
}

int stack_push(IntStack *s, int value) {
    if (s->size >= INT_STACK_CAP) return -1;
    s->data[s->size++] = value;
    return 0;
}

int stack_pop(IntStack *s, int *out) {
    if (s->size == 0) return -1;
    *out = s->data[--s->size];
    return 0;
}

int stack_peek(const IntStack *s, int *out) {
    if (s->size == 0) return -1;
    *out = s->data[s->size - 1];
    return 0;
}

int stack_size(const IntStack *s) {
    return (int)s->size;
}
