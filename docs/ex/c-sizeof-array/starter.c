#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int arr[10];
    for (int k = 0; k < n && k < 10; ++k) {
        if (scanf("%d", &arr[k]) != 1) arr[k] = 0;
    }

    /* TODO: 用 sizeof 求出数组能放几个元素，再求出一个 int 占几个字节。 */
    printf("0 0\n");
    return 0;
}
