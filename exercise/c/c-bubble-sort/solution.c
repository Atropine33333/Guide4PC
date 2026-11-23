// 参考实现：c-bubble-sort
#include <stdio.h>

#define MAX_N 1000

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 1 || n > MAX_N) return 0;

    static int a[MAX_N];
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1) return 0;
    }

    for (int end = n - 1; end > 0; --end) {
        int swapped = 0;
        for (int i = 0; i < end; ++i) {
            if (a[i] > a[i + 1]) {
                int tmp = a[i];
                a[i] = a[i + 1];
                a[i + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }

    for (int i = 0; i < n; ++i) {
        if (i > 0) putchar(' ');
        printf("%d", a[i]);
    }
    putchar('\n');
    return 0;
}
