#include <stdio.h>

int main(void) {
    double price = 0.0;
    if (scanf("%lf", &price) != 1) return 0;

    /* TODO: 小票价保留两位小数，抹零价只留整数位。 */
    (void)price;
    printf("0.00 0\n");
    return 0;
}
