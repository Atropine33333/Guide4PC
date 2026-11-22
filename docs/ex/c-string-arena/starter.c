#include <stddef.h>

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
    /* TODO: 一次申请 bytes 字节，base 指向它，cap 记下大小，used 归零。
       bytes 为 0 或申请失败返回 -1。 */
    (void)a;
    (void)bytes;
    return -1;
}

void *arena_alloc(Arena *a, size_t n) {
    /* TODO: 从 used 处切 n 字节出来，返回切片首地址并把 used 往后推。
       放不下或 n 为 0 返回 NULL，used 不动。 */
    (void)a;
    (void)n;
    return NULL;
}

char *arena_strdup(Arena *a, const char *s) {
    /* TODO: 把 s 连同 '\0' 放进营地，返回新地址；放不下返回 NULL。 */
    (void)a;
    (void)s;
    return NULL;
}

void arena_reset(Arena *a) {
    /* TODO: used 归零，已申请的那块地不动。 */
    (void)a;
}

void arena_free(Arena *a) {
    /* TODO: 归还整块地，base 置 NULL，cap 与 used 置 0。 */
    (void)a;
}

size_t arena_used(const Arena *a) {
    /* TODO */
    (void)a;
    return 0;
}
