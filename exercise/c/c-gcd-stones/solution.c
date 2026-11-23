// 参考实现：c-gcd-stones
#include <stdio.h>

static long long gcd_ll(long long x, long long y) {
    while (y != 0) {
        long long r = x % y;
        x = y;
        y = r;
    }
    return x;
}

int main(void) {
    long long a = 0;
    long long b = 0;
    if (scanf("%lld %lld", &a, &b) != 2) return 0;

    long long g = gcd_ll(a, b);
    long long l = a / g * b;

    printf("%lld %lld\n", g, l);
    return 0;
}
