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
    /* TODO: 所有格子标成空闲，in_use 置 0。 */
    (void)p;
}

void *pool_alloc(Pool *p) {
    /* TODO: 返回下标最小的空闲格子并标记为占用；没有空格子或 p 为 NULL 时返回 NULL。 */
    (void)p;
    return NULL;
}

int pool_free(Pool *p, void *block) {
    /* TODO: block 是某个格子的首地址且正被占用时归还并返回 0；
       否则（野指针、重复归还、NULL）返回 -1。 */
    (void)p;
    (void)block;
    return -1;
}

void pool_reset(Pool *p) {
    /* TODO: 把所有格子收回，in_use 归零。 */
    (void)p;
}

int pool_in_use(const Pool *p) {
    /* TODO */
    (void)p;
    return 0;
}
