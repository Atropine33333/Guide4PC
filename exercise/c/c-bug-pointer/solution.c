// 参考实现：c-bug-pointer
#include <stddef.h>

void swap_ints(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int *max_ptr(int *a, int n) {
    int *best = a;
    for (int i = 1; i < n; ++i) {
        if (a[i] > *best) {
            best = a + i;
        }
    }
    return best;
}
