#include <stdio.h>

int main(void) {
    long long sum = 0;
    int count = 0;
    int value = 0;

    /* TODO: 先把第一枚读进来；读到 0 就结束；
       非零的值计入个数与总和。目前只读不记。 */
    do {
        if (scanf("%d", &value) != 1) break;
        (void)value;
    } while (value != 0);

    printf("%d %lld\n", count, sum);
    return 0;
}
