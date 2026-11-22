// 参考实现：c-string-arena
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *base;
    size_t cap;
    size_t used;
} Arena;

int arena_init(Arena *a, size_t bytes);
void *arena_alloc(Arena *a, size_t n);
char *arena_strdup(Arena *a, const char *s);
void arena_reset(Arena *a);
void arena_free(Arena *a);
size_t arena_used(const Arena *a);

int arena_init(Arena *a, size_t bytes) {
    if (a == NULL) return -1;
    a->base = NULL;
    a->cap = 0;
    a->used = 0;
    if (bytes == 0) return -1;
    char *p = (char *)malloc(bytes);
    if (p == NULL) return -1;
    a->base = p;
    a->cap = bytes;
    return 0;
}

void *arena_alloc(Arena *a, size_t n) {
    if (a == NULL || a->base == NULL || n == 0) return NULL;
    if (n > a->cap - a->used) return NULL;
    char *p = a->base + a->used;
    a->used += n;
    return p;
}

char *arena_strdup(Arena *a, const char *s) {
    if (a == NULL || s == NULL) return NULL;
    size_t n = strlen(s) + 1;
    char *p = (char *)arena_alloc(a, n);
    if (p == NULL) return NULL;
    memcpy(p, s, n);
    return p;
}

void arena_reset(Arena *a) {
    if (a != NULL) a->used = 0;
}

void arena_free(Arena *a) {
    if (a == NULL) return;
    free(a->base);
    a->base = NULL;
    a->cap = 0;
    a->used = 0;
}

size_t arena_used(const Arena *a) {
    return a == NULL ? 0 : a->used;
}
