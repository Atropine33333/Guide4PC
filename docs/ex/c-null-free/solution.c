// 参考实现：c-null-free
#include <stddef.h>
#include <stdlib.h>

void safe_free(void **p) {
    if (p == NULL) return;
    if (*p == NULL) return;
    free(*p);
    *p = NULL;
}
