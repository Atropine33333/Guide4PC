// 参考实现：c-memmove-overlap
#include <stddef.h>
#include <string.h>

void shift_left(int *a, int n, int k) {
    if (n <= 0 || k <= 0) return;
    if (k >= n) {
        memset(a, 0, (size_t)n * sizeof *a);
        return;
    }
    memmove(a, a + k, (size_t)(n - k) * sizeof *a);
    memset(a + (n - k), 0, (size_t)k * sizeof *a);
}
