#include <stddef.h>

#define ARENA_CAP 256

typedef struct {
    _Alignas(8) unsigned char buf[ARENA_CAP];
    size_t used;
} Arena;

void arena_init(Arena *a);
void *arena_alloc(Arena *a, size_t n);
void arena_reset(Arena *a);
size_t arena_used(const Arena *a);

void arena_init(Arena *a) {
    /* TODO: 已用置 0。 */
    (void)a;
}

void *arena_alloc(Arena *a, size_t n) {
    /* TODO: 从 used 处按 8 字节取整切一块；放不下返回 NULL，used 不变。 */
    (void)a;
    (void)n;
    return NULL;
}

void arena_reset(Arena *a) {
    /* TODO: 已用归 0，旧字节不擦。 */
    (void)a;
}

size_t arena_used(const Arena *a) {
    /* TODO: 返回当前已用字节数。 */
    (void)a;
    return 0;
}
