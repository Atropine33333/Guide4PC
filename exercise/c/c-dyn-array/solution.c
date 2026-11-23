// 参考实现：c-dyn-array
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t cap;
} IntVec;

void vec_init(IntVec *v) {
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}

int vec_push(IntVec *v, int value) {
    if (v->size == v->cap) {
        size_t next = v->cap == 0 ? 4 : v->cap * 2;
        int *fresh = (int *)realloc(v->data, next * sizeof(int));
        if (fresh == NULL) return -1;
        v->data = fresh;
        v->cap = next;
    }
    v->data[v->size++] = value;
    return 0;
}

void vec_free(IntVec *v) {
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}
