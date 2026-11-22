// 参考实现：c-aligned-alloc
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

void *alloc_aligned64(size_t n);
void free_aligned64(void *p);

void *alloc_aligned64(size_t n) {
    const size_t align = 64;
    if (n == 0) return NULL;
    size_t extra = align - 1 + sizeof(void *);
    if (n > SIZE_MAX - extra) return NULL;
    unsigned char *raw = (unsigned char *)malloc(n + extra);
    if (raw == NULL) return NULL;
    uintptr_t start = (uintptr_t)(raw + sizeof(void *));
    uintptr_t aligned = (start + (align - 1)) & ~(uintptr_t)(align - 1);
    void **slot = (void **)aligned;
    slot[-1] = raw;
    return (void *)slot;
}

void free_aligned64(void *p) {
    if (p == NULL) return;
    void **slot = (void **)p;
    free(slot[-1]);
}
