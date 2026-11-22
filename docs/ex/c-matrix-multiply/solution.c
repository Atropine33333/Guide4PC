// 参考实现：c-matrix-multiply
#include <stdio.h>

#define MAX_DIM 50

int main(void) {
    int n, m, k;
    if (scanf("%d %d %d", &n, &m, &k) != 3) return 0;
    if (n < 1 || n > MAX_DIM || m < 1 || m > MAX_DIM || k < 1 || k > MAX_DIM) return 0;

    static int a[MAX_DIM][MAX_DIM];
    static int b[MAX_DIM][MAX_DIM];
    static int c[MAX_DIM][MAX_DIM];

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

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            int sum = 0;
            for (int t = 0; t < m; ++t) {
                sum += a[i][t] * b[t][j];
            }
            c[i][j] = sum;
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            if (j > 0) putchar(' ');
            printf("%d", c[i][j]);
        }
        putchar('\n');
    }
    return 0;
}
