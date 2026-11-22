// 参考实现：c-sizeof-array
#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int arr[10];
    for (int k = 0; k < n && k < 10; ++k) {
        if (scanf("%d", &arr[k]) != 1) arr[k] = 0;
    }

    int slots = (int)(sizeof arr / sizeof arr[0]);
    int width = (int)sizeof(int);
    printf("%d %d\n", slots, width);
    return 0;
}
