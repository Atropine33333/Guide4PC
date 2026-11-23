// 参考实现：c-heap-sift
#include <stddef.h>

#define HEAP_CAP 64

typedef struct {
    int data[HEAP_CAP];
    size_t len;
} MinHeap;

static void swap_int(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void heap_sift_down(MinHeap *h, size_t i) {
    for (;;) {
        size_t left = 2 * i + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < h->len && h->data[left] < h->data[smallest]) smallest = left;
        if (right < h->len && h->data[right] < h->data[smallest]) smallest = right;
        if (smallest == i) break;
        swap_int(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
}

void heap_build(MinHeap *h, const int *values, size_t n) {
    if (n > HEAP_CAP) n = HEAP_CAP;
    for (size_t i = 0; i < n; ++i) h->data[i] = values[i];
    h->len = n;
    for (size_t i = n / 2; i-- > 0;) heap_sift_down(h, i);
}

int heap_extract_min(MinHeap *h, int *out) {
    if (h->len == 0) return -1;
    *out = h->data[0];
    h->len -= 1;
    if (h->len > 0) {
        h->data[0] = h->data[h->len];
        heap_sift_down(h, 0);
    }
    return 0;
}
