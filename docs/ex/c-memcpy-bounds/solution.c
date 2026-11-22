// 参考实现：c-memcpy-bounds
#include <stddef.h>
#include <string.h>

size_t copy_capped(char *dst, size_t cap, const char *src, size_t n) {
    if (dst == NULL || src == NULL || cap == 0) return 0;
    size_t room = cap - 1;
    size_t count = (n < room) ? n : room;
    memcpy(dst, src, count);
    dst[count] = '\0';
    return count;
}
