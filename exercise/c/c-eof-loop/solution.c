// 参考实现：c-eof-loop
#include <stdio.h>

int main(void) {
    long long count = 0;
    long long sum = 0;
    long long x;

    while (scanf("%lld", &x) == 1) {
        ++count;
        sum += x;
    }

    printf("%lld %lld\n", count, sum);
    return 0;
}
