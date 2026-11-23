// 参考实现：c-realloc-grow
#include <stddef.h>
#include <stdlib.h>

int append(int **a, int *n, int *cap, int v) {
    if (*n == *cap) {
        int newcap = (*cap == 0) ? 4 : *cap * 2;
        int *grown = realloc(*a, (size_t)newcap * sizeof(int));
        if (grown == NULL) return -1;
        *a = grown;
        *cap = newcap;
    }
    (*a)[*n] = v;
    *n = *n + 1;
    return 0;
}
