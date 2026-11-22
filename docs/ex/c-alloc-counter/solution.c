// 参考实现：c-alloc-counter
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    size_t allocs;
    size_t frees;
} AllocStats;

void ac_init(AllocStats *st);
void *ac_alloc(AllocStats *st, size_t size);
void ac_free(AllocStats *st, void *p);
long ac_outstanding(const AllocStats *st);

void ac_init(AllocStats *st) {
    if (st == NULL) return;
    st->allocs = 0;
    st->frees = 0;
}

void *ac_alloc(AllocStats *st, size_t size) {
    if (st == NULL || size == 0) return NULL;
    void *p = malloc(size);
    if (p != NULL) ++st->allocs;
    return p;
}

void ac_free(AllocStats *st, void *p) {
    if (st == NULL || p == NULL) return;
    free(p);
    ++st->frees;
}

long ac_outstanding(const AllocStats *st) {
    if (st == NULL || st->allocs < st->frees) return 0;
    return (long)(st->allocs - st->frees);
}
