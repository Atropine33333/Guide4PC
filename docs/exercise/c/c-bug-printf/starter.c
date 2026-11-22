#include <stdio.h>

int main(void) {
    double a, b;
    if (scanf("%lf %lf", &a, &b) != 2) return 0;

    double sum = a + b;
    printf("%d\n", sum);
    return 0;
}
