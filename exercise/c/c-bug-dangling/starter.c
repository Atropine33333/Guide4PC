#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define POOL_CAP 8
#define SLOT_LEN 32

typedef struct {
    char slots[POOL_CAP][SLOT_LEN];
    unsigned char used[POOL_CAP];
} NamePool;

void pool_init(NamePool *p);
const char *pool_register(NamePool *p, const char *name);
void pool_release(NamePool *p, const char *slot);

void pool_init(NamePool *p) {
    for (int i = 0; i < POOL_CAP; ++i) {
        p->used[i] = 0;
    }
}

void pool_release(NamePool *p, const char *slot) {
    uintptr_t base = (uintptr_t)(const void *)p->slots;
    uintptr_t at = (uintptr_t)(const void *)slot;
    if (at < base || at >= base + sizeof p->slots) return;
    size_t offset = (size_t)(at - base);
    if (offset % SLOT_LEN != 0) return;
    p->used[offset / SLOT_LEN] = 0;
}

const char *pool_register(NamePool *p, const char *name) {
    int idx = -1;
    for (int i = 0; i < POOL_CAP; ++i) {
        if (!p->used[i]) {
            idx = i;
            break;
        }
    }
    if (idx < 0) return NULL;

    snprintf(p->slots[idx], SLOT_LEN, "%s", name);
    pool_release(p, p->slots[idx]); /* 坏在这里：抄完就把舱位交还了 */
    return p->slots[idx];
}
