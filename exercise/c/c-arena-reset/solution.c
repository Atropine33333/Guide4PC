// 参考实现：c-arena-reset
#include <stddef.h>

#define ARENA_CAP 256
#define ARENA_ALIGN 8

typedef struct {
    _Alignas(8) unsigned char buf[ARENA_CAP];
    size_t used;
} Arena;

void arena_init(Arena *a);
void *arena_alloc(Arena *a, size_t n);
void arena_reset(Arena *a);
size_t arena_used(const Arena *a);

void arena_init(Arena *a) {
    if (a != NULL) a->used = 0;
}

void *arena_alloc(Arena *a, size_t n) {
    if (a == NULL || n == 0 || n > ARENA_CAP) return NULL;
    size_t need = ((n + ARENA_ALIGN - 1) / ARENA_ALIGN) * ARENA_ALIGN;
    if (need > ARENA_CAP - a->used) return NULL;
    void *p = a->buf + a->used;
    a->used += need;
    return p;
}

void arena_reset(Arena *a) {
    if (a != NULL) a->used = 0;
}

size_t arena_used(const Arena *a) {
    return a == NULL ? 0 : a->used;
}
