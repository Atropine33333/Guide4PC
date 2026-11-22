#include <stdio.h>

/* 摆到第 row 行时，cols、d1、d2 记录已被占的列与两条对角线。 */
static long long count_from(int n, int row, unsigned cols, unsigned d1, unsigned d2) {
    (void)n;
    (void)row;
    (void)cols;
    (void)d1;
    (void)d2;
    /* TODO: 已经摆满 n 行就返回 1；否则逐列试放，把后续摆法数累加起来。 */
    return 0;
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
