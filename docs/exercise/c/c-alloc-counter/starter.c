#include <stddef.h>

typedef struct {
    size_t allocs;
    size_t frees;
} AllocStats;

void ac_init(AllocStats *st);
void *ac_alloc(AllocStats *st, size_t size);
void ac_free(AllocStats *st, void *p);
long ac_outstanding(const AllocStats *st);

void ac_init(AllocStats *st) {
    /* TODO: 把两项计数都置 0。 */
    (void)st;
}

void *ac_alloc(AllocStats *st, size_t size) {
    /* TODO: 申请 size 字节；成功时 allocs 加一，失败或 size 为 0 返回 NULL 且不记账。 */
    (void)st;
    (void)size;
    return NULL;
}

void ac_free(AllocStats *st, void *p) {
    /* TODO: 归还 p；p 为 NULL 时什么也不做，否则 frees 加一。 */
    (void)st;
    (void)p;
}

long ac_outstanding(const AllocStats *st) {
    /* TODO: 返回 allocs 与 frees 的差。 */
    (void)st;
    return 0;
}
