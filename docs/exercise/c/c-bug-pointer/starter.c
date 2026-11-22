#include <stddef.h>

void swap_ints(int *a, int *b) {
    /* BUG: 这里动的只是两个指针自己的指向。 */
    int *t = a;
    a = b;
    b = t;
}

int *max_ptr(int *a, int n) {
    /* BUG: 比的是地址，忘了比地址里的值。 */
    int *best = a;
    for (int i = 1; i < n; ++i) {
        if (a + i > best) {
            best = a + i;
        }
    }
    return best;
}
