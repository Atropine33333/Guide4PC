#include <stdio.h>

int main(void) {
    long long count = 0;
    long long sum = 0;
    long long x;

    while (scanf("%lld", &x) == 1) {
        /* TODO：把 x 记进个数与总和。 */
        (void)x;
    }

    /* 目前不管读到什么，都会打印 0 0。 */
    printf("%lld %lld\n", count, sum);
    return 0;
}
