#include <stdio.h>

#define MAX_N 1000

int main(void) {
    static int a[MAX_N];
    static int b[MAX_N];

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

    /* TODO: 两个数组都已升序。用两个下标 i、j 分别指向 a、b 的队头，
       每次把较小的那个放进结果、下标后移；某一队走空以后，
       另一队剩下的依次接上。 */

    for (int i = 0; i < n; ++i) {
        if (i > 0) putchar(' ');
        printf("%d", a[i]);
    }
    for (int j = 0; j < m; ++j) {
        if (n > 0 || j > 0) putchar(' ');
        printf("%d", b[j]);
    }
    putchar('\n');
    return 0;
}
