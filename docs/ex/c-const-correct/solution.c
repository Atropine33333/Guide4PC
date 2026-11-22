// 参考实现：c-const-correct
#include <stddef.h>

size_t str_len(const char *s) {
    if (s == NULL) return 0;

    size_t n = 0;
    while (s[n] != '\0') {
        ++n;
    }
    return n;
}
