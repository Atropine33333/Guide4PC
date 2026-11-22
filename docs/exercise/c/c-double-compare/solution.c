// 参考实现：c-double-compare
#include <stdio.h>

int main(void) {
    double a = 0.0;
    double b = 0.0;
    if (scanf("%lf %lf", &a, &b) != 2) return 0;

    double gap = a - b;
    if (gap < 0.0) gap = -gap;

    printf("%d\n", gap < 1e-9);
    printf("%d\n", 0.1 + 0.2 == 0.3);
    return 0;
}
