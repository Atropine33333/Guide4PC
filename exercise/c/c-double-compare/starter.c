#include <stdio.h>

int main(void) {
    double a = 0.0;
    double b = 0.0;
    if (scanf("%lf %lf", &a, &b) != 2) return 0;

    /* TODO: 用容差比较 a 与 b，差小于 1e-9 输出 1，否则输出 0。 */
    (void)a;
    (void)b;
    printf("0\n");

    /* TODO: 直接比较 0.1 + 0.2 与 0.3，相等输出 1，否则输出 0。 */
    printf("0\n");
    return 0;
}
