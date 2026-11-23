// 参考实现：c-calloc-zero
#include <stddef.h>
#include <stdlib.h>

int *zeros(size_t n) {
    if (n == 0) return NULL;
    return calloc(n, sizeof(int));
}
