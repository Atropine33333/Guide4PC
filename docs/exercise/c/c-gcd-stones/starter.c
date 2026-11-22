#include <stdio.h>

int main(void) {
    long long a = 0;
    long long b = 0;
    if (scanf("%lld %lld", &a, &b) != 2) return 0;

    long long x = a;
    long long y = b;
    long long g = 0;
    long long l = 0;

    /* TODO: 用更相减损或辗转相除，把 g 求成 x 与 y 的最大公约数。 */
    (void)x;
    (void)y;

    /* TODO: 先除后乘，把最小公倍数存进 l。 */

    printf("%lld %lld\n", g, l);
    return 0;
}
