// 参考实现：c-pool-alloc
#include <stddef.h>

#define POOL_BLOCK_SIZE 32
#define POOL_BLOCK_COUNT 8

typedef struct {
    unsigned char cells[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];
    unsigned char used[POOL_BLOCK_COUNT];
    int in_use;
} Pool;

void pool_init(Pool *p);
void *pool_alloc(Pool *p);
int pool_free(Pool *p, void *block);
void pool_reset(Pool *p);
int pool_in_use(const Pool *p);

void pool_init(Pool *p) {
    if (p == NULL) return;
    for (int i = 0; i < POOL_BLOCK_COUNT; ++i) p->used[i] = 0;
    p->in_use = 0;
}

void *pool_alloc(Pool *p) {
    if (p == NULL) return NULL;
    for (int i = 0; i < POOL_BLOCK_COUNT; ++i) {
        if (!p->used[i]) {
            p->used[i] = 1;
            ++p->in_use;
            return (void *)p->cells[i];
        }
    }
    return NULL;
}

int pool_free(Pool *p, void *block) {
    if (p == NULL || block == NULL) return -1;
    for (int i = 0; i < POOL_BLOCK_COUNT; ++i) {
        if ((void *)p->cells[i] == block) {
            if (!p->used[i]) return -1;
            p->used[i] = 0;
            --p->in_use;
            return 0;
        }
    }
    return -1;
}

void pool_reset(Pool *p) {
    pool_init(p);
}

int pool_in_use(const Pool *p) {
    return p == NULL ? 0 : p->in_use;
}
