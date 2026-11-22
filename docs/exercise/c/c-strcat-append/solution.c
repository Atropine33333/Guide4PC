// 参考实现：c-strcat-append
#include <stddef.h>
#include <string.h>

int append_str(char *buf, size_t cap, const char *s) {
    if (cap == 0) return -1;
    size_t used = strlen(buf);
    size_t extra = strlen(s);
    if (used + extra + 1 > cap) return -1;
    memcpy(buf + used, s, extra + 1);
    return 0;
}
