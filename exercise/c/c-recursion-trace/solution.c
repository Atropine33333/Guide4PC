// 参考实现：c-recursion-trace
#include <stdio.h>

static long long fact(int n, int *calls) {
    ++(*calls);
    if (n == 0) return 1;
    return (long long)n * fact(n - 1, calls);
}

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int calls = 0;
    long long value = fact(n, &calls);
    printf("%lld %d\n", value, calls);
    return 0;
}
