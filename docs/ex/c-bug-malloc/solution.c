// 参考实现：c-bug-malloc
#include <stddef.h>
#include <stdlib.h>

#define SQ_MAX 4096

static long g_live = 0;

int *make_squares(int n) {
    if (n < 1 || n > SQ_MAX) {
        return NULL;
    }
    int *block = (int *)malloc((size_t)n * sizeof(int));
    if (block == NULL) {
        return NULL;
    }
    for (int i = 0; i < n; ++i) {
        block[i] = i * i;
    }
    ++g_live;
    return block;
}

void release_squares(int *p) {
    if (p == NULL) {
        return;
    }
    free(p);
    --g_live;
}

long live_blocks(void) {
    return g_live;
}
