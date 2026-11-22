#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 0 || n > 100) return 0;

    for (int i = 0; i < n; ++i) {
        long long a;
        long long b;
        if (scanf("%lld %lld", &a, &b) != 2) return 0;

        /* TODO: 输出 a + b。 */
        (void)a;
        (void)b;
    }
    return 0;
}
