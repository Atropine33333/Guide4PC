// 参考实现：c-nqueens-count
#include <stdio.h>

static long long count_from(int n, int row, unsigned cols, unsigned d1, unsigned d2) {
    if (row == n) return 1;

    unsigned full = (1u << n) - 1u;
    unsigned free_cols = full & ~(cols | d1 | d2);
    long long total = 0;

    while (free_cols != 0u) {
        unsigned bit = free_cols & (~free_cols + 1u);
        free_cols ^= bit;
        total += count_from(n, row + 1, cols | bit, ((d1 | bit) << 1) & full, (d2 | bit) >> 1);
    }

    return total;
}

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    long long total = 0;
    if (n >= 1 && n <= 10) {
        total = count_from(n, 0, 0u, 0u, 0u);
    }
    printf("%lld\n", total);
    return 0;
}
