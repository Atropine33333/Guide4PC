// 参考实现：c-extended-gcd
#include <stdio.h>

static long long ext_gcd(long long a, long long b, long long *x, long long *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }

    long long x1 = 0;
    long long y1 = 0;
    long long g = ext_gcd(b, a % b, &x1, &y1);

    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

int main(void) {
    long long a = 0;
    long long b = 0;
    if (scanf("%lld %lld", &a, &b) != 2) return 0;
    if (a <= 0 || b <= 0) return 0;

    long long x = 0;
    long long y = 0;
    long long g = ext_gcd(a, b, &x, &y);

    long long step = b / g;
    x %= step;
    if (x < 0) x += step;
    y = (g - a * x) / b;

    printf("%lld %lld %lld\n", g, x, y);
    return 0;
}
