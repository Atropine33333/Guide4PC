// 参考实现：c-merge-sorted
#include <stdio.h>

#define MAX_N 1000

int main(void) {
    static int a[MAX_N];
    static int b[MAX_N];
    static int merged[2 * MAX_N];

    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 0 || n > MAX_N) return 0;
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1) return 0;
    }

    int m;
    if (scanf("%d", &m) != 1) return 0;
    if (m < 0 || m > MAX_N) return 0;
    for (int j = 0; j < m; ++j) {
        if (scanf("%d", &b[j]) != 1) return 0;
    }
    if (n + m < 1) return 0;

    int i = 0;
    int j = 0;
    int t = 0;
    while (i < n && j < m) {
        if (a[i] <= b[j]) {
            merged[t++] = a[i++];
        } else {
            merged[t++] = b[j++];
        }
    }
    while (i < n) merged[t++] = a[i++];
    while (j < m) merged[t++] = b[j++];

    for (int q = 0; q < t; ++q) {
        if (q > 0) putchar(' ');
        printf("%d", merged[q]);
    }
    putchar('\n');
    return 0;
}
