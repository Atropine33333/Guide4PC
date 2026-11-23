#include <stddef.h>
#include <stdlib.h>

#define SQ_MAX 4096

static long g_live = 0;

int *make_squares(int n) {
    /* BUG: 批没批下地来，这里不看一眼就往下写。 */
    int *block = (int *)malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        block[i] = i * i;
    }
    ++g_live;
    return block;
}

void release_squares(int *p) {
    /* BUG: 归还这道工序空着，账也不销。 */
    (void)p;
}

long live_blocks(void) {
    return g_live;
}
