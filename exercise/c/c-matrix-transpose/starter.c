#include <stdio.h>

int main(void) {
    int n = 0;
    int m = 0;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    int a[50][50];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (scanf("%d", &a[i][j]) != 1) return 0;
        }
    }

    (void)a;

    /* TODO: 输出转置，共 m 行，每行 n 个数。 */
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            if (i > 0) putchar(' ');
            printf("%d", 0);
        }
        putchar('\n');
    }
    return 0;
}
