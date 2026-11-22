#include <stdio.h>

static long long fact(int n, int *calls) {
    /* TODO: 每进入一次就把 *calls 加 1；n 为 0 时返回 1，
       否则返回 n 与 fact(n - 1, calls) 的乘积。 */
    (void)n;
    (void)calls;
    return 1;
}

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int calls = 0;
    long long value = fact(n, &calls);
    printf("%lld %d\n", value, calls);
    return 0;
}
