#include <stdio.h>

/* 求 a、b 的最大公约数；x、y 带回 a*x + b*y = gcd 的一组解。 */
static long long ext_gcd(long long a, long long b, long long *x, long long *y) {
    (void)a;
    (void)b;
    *x = 1;
    *y = 0;
    /* TODO: b 为 0 时返回 a；否则递归求 b 与 a % b 的解，再回代换算出本层的 x、y。 */
    return 0;
}

int main(void) {
    long long a = 0;
    long long b = 0;
    if (scanf("%lld %lld", &a, &b) != 2) return 0;

    long long x = 0;
    long long y = 0;
    long long g = ext_gcd(a, b, &x, &y);

    printf("%lld %lld %lld\n", g, x, y);
    return 0;
}
