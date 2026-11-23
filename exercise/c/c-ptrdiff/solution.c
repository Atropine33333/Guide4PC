// 参考实现：c-ptrdiff
#include <stdio.h>
#include <stddef.h>

int main(void) {
    int n = 0;
    int arr[105];
    if (scanf("%d", &n) != 1 || n < 1) return 0;
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &arr[i]) != 1) return 0;
    }

    ptrdiff_t gap = &arr[n - 1] - &arr[0];
    printf("%td %d\n", gap, arr[n / 2]);
    return 0;
}
