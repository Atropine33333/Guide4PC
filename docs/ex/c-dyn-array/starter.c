#include <stddef.h>

typedef struct {
    int *data;
    size_t size;
    size_t cap;
} IntVec;

void vec_init(IntVec *v);
int vec_push(IntVec *v, int value);
void vec_free(IntVec *v);

void vec_init(IntVec *v) {
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}

int vec_push(IntVec *v, int value) {
    /* TODO: 容量不够时扩容，追加 value；成功返回 0，失败返回 -1。 */
    (void)v;
    (void)value;
    return -1;
}

void vec_free(IntVec *v) {
    /* TODO: 释放 data 指向的内存，并把 size 置 0。 */
    (void)v;
}
