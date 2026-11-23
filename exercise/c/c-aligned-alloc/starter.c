#include <stddef.h>

void *alloc_aligned64(size_t n);
void free_aligned64(void *p);

void *alloc_aligned64(size_t n) {
    /* TODO: 申请至少 n 字节，返回地址是 64 的整数倍；n 为 0 返回 NULL。 */
    (void)n;
    return NULL;
}

void free_aligned64(void *p) {
    /* TODO: 归还 alloc_aligned64 拿到的那块内存；p 为 NULL 时什么也不做。 */
    (void)p;
}
