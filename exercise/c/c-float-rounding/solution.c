// 参考实现：c-float-rounding
#include <stdio.h>

int main(void) {
    double price = 0.0;
    if (scanf("%lf", &price) != 1) return 0;

    printf("%.2f %.0f\n", price, price);
    return 0;
}
