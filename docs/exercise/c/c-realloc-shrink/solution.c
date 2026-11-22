// 参考实现：c-realloc-shrink
#include <stddef.h>
#include <stdlib.h>

int trim_to(int **a, int *cap, int n) {
    if (a == NULL || cap == NULL) return -1;
    if (n < 0 || n > *cap) return -1;
    if (n == 0) {
        free(*a);
        *a = NULL;
        *cap = 0;
        return 0;
    }
    if (n == *cap) return 0;
    int *shrunk = (int *)realloc(*a, (size_t)n * sizeof(int));
    if (shrunk == NULL) return -1;
    *a = shrunk;
    *cap = n;
    return 0;
}
