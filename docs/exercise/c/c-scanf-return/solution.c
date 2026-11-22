// 参考实现：c-scanf-return
#include <stdio.h>

int read_pair(const char *s, int *a, int *b) {
    if (s == NULL) return 0;

    int got = sscanf(s, "%d %d", a, b);
    if (got < 0) return 0;
    return got;
}
