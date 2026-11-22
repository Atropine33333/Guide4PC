// 参考实现：c-array-decay
#include <stdio.h>

#define MAX_N 1000

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_N) return 0;

    int a[MAX_N];
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1) return 0;
    }

    long long sum = 0;
    int best = a[0];

    const int *p = a;
    const int *end = a + n;
    for (; p < end; ++p) {
        sum += *p;
        if (*p > best) {
            best = *p;
        }
    }

    printf("%lld %d\n", sum, best);
    return 0;
}
