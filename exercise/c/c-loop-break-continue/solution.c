// 参考实现：c-loop-break-continue
#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int sum = 0;
    for (int i = 1; i <= n; ++i) {
        if (i % 3 == 0) continue;
        sum += i;
        if (sum > 100) break;
    }

    printf("%d\n", sum);
    return 0;
}
