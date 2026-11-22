#include <stdio.h>
#include <stddef.h>

int main(void) {
    int n = 0;
    int arr[105];
    if (scanf("%d", &n) != 1 || n < 1) return 0;
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &arr[i]) != 1) return 0;
    }

    /* TODO: 求 &arr[n - 1] - &arr[0]，再取 arr[n / 2]，
       两个结果用一个空格隔开输出。 */
    printf("0 0\n");
    return 0;
}
