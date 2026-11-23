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

    /* TODO: 用冒泡排序把 a[0] 到 a[n-1] 排成升序。
       从左到右两两相比，前面的大就交换；走完一趟，
       最大的落在末尾，下一趟少看末尾这一格。 */

    for (int i = 0; i < n; ++i) {
        if (i > 0) putchar(' ');
        printf("%d", a[i]);
    }
    putchar('\n');
    return 0;
}
