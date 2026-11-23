// 参考实现：c-division-rules
#include <stdio.h>

int main(void) {
    int a = 0;
    int b = 0;
    if (scanf("%d %d", &a, &b) != 2 || b == 0) return 0;

    int q = a / b;
    int r = a % b;

    printf("%d %d\n", q, r);
    return 0;
}
