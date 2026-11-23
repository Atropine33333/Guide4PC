// 参考实现：c-fast-pow
#include <stdio.h>

int main(void) {
    long long b = 0;
    long long e = 0;
    long long m = 0;
    if (scanf("%lld %lld %lld", &b, &e, &m) != 3) return 0;

    long long result = 1 % m;
    long long base = b % m;

    while (e > 0) {
        if (e & 1) result = result * base % m;
        base = base * base % m;
        e >>= 1;
    }

    printf("%lld\n", result);
    return 0;
}
