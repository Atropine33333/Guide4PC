// 参考实现：c-bug-printf
#include <stdio.h>

int main(void) {
    double a, b;
    if (scanf("%lf %lf", &a, &b) != 2) return 0;

    double sum = a + b;
    printf("%.2f\n", sum);
    return 0;
}
