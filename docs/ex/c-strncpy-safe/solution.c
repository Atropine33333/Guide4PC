// 参考实现：c-strncpy-safe
#include <stddef.h>

void copy_name(char *dst, size_t cap, const char *s) {
    if (cap == 0) return;
    size_t i = 0;
    while (i + 1 < cap && s[i] != '\0') {
        dst[i] = s[i];
        ++i;
    }
    dst[i] = '\0';
}
