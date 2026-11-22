#include <stdio.h>

#define MAX_DIM 50

int main(void) {
    int n, m, k;
    if (scanf("%d %d %d", &n, &m, &k) != 3) return 0;
    if (n < 1 || n > MAX_DIM || m < 1 || m > MAX_DIM || k < 1 || k > MAX_DIM) return 0;

    static int a[MAX_DIM][MAX_DIM];
    static int b[MAX_DIM][MAX_DIM];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (scanf("%d", &a[i][j]) != 1) return 0;
        }
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) {
            if (scanf("%d", &b[i][j]) != 1) return 0;
        }
    }

    /* TODO: 三重循环求 C = A × B。
       C 的第 i 行第 j 列，是 A 的第 i 行与 B 的第 j 列
       逐项相乘再相加。 */

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            if (j > 0) putchar(' ');
            printf("%d", 0);
        }
        putchar('\n');
    }
    return 0;
}
