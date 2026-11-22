// 参考实现：c-short-circuit
#include <stdio.h>

int main(void) {
    int a = 0;
    int b = 0;
    if (scanf("%d %d", &a, &b) != 2) return 0;

    int first = (b != 0 && a / b > 2);
    int second = (b == 0 || a / b > 2);

    printf("%d %d\n", first, second);
    return 0;
}
