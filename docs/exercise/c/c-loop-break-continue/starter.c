#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int sum = 0;
    for (int i = 1; i <= n; ++i) {
        /* TODO: 三的倍数跳过；其余加进 sum，加完立刻看是否超过 100。 */
        sum += i;
        if (sum > 100) break;
    }

    printf("%d\n", sum);
    return 0;
}
