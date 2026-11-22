// 参考实现：c-sorted-insert
#include <stddef.h>

#define ARR_CAP 64

typedef struct {
    int data[ARR_CAP];
    int size;
} SortedArray;

void sa_init(SortedArray *a) {
    a->size = 0;
}

int sa_insert(SortedArray *a, int value) {
    if (a->size >= ARR_CAP) return -1;
    int i = a->size;
    while (i > 0 && a->data[i - 1] > value) {
        a->data[i] = a->data[i - 1];
        --i;
    }
    a->data[i] = value;
    ++a->size;
    return 0;
}

int sa_size(const SortedArray *a) {
    return a->size;
}
