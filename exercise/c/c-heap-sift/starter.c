#include <stddef.h>

#define HEAP_CAP 64

typedef struct {
    int data[HEAP_CAP];
    size_t len;
} MinHeap;

void heap_sift_down(MinHeap *h, size_t i);
void heap_build(MinHeap *h, const int *values, size_t n);
int heap_extract_min(MinHeap *h, int *out);

void heap_sift_down(MinHeap *h, size_t i) {
    /* TODO: 与更小的孩子交换，一路下沉到不能再沉。 */
    (void)h;
    (void)i;
}

void heap_build(MinHeap *h, const int *values, size_t n) {
    /* TODO: 先抄进 data，再从最后一个非叶节点往前逐个下沉。 */
    (void)h;
    (void)values;
    (void)n;
}

int heap_extract_min(MinHeap *h, int *out) {
    /* TODO: 取走堆顶，把最后一个元素搬到堆顶再下沉；空堆返回 -1。 */
    (void)h;
    (void)out;
    return -1;
}
