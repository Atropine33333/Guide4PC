// 参考实现：c-format-padding
#include <stdio.h>

int main(void) {
    int width = 0;
    int n = 0;
    if (scanf("%d %d", &width, &n) != 2) return 0;

    printf("[%*d]\n", width, n);
    return 0;
}
