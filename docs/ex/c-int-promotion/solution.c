// 参考实现：c-int-promotion
#include <stdio.h>

int main(void) {
    int a = 0;
    int b = 0;
    if (scanf("%d %d", &a, &b) != 2) return 0;

    int sum = (char)a + (char)b;
    int width = (int)sizeof(a + b);
    int clipped = (unsigned char)300;

    printf("%d %d %d\n", sum, width, clipped);
    return 0;
}
