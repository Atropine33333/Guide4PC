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

    /* TODO: 让一个指针从 a 的首元素出发，一步一步走到 a + n，
       沿途把每个 *p 加进 sum，并刷新 best。 */
    (void)sum;
    (void)best;

    printf("%lld %d\n", sum, best);
    return 0;
}
