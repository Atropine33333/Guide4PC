#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    long long sum = 0;
    int count = 0;

    /* TODO: 在 for 的步进部分用逗号表达式同时做三件事：
       把当前项加进 sum、让 count 加一、让循环变量走向下一项。 */

    printf("%lld %d\n", sum, count);
    return 0;
}
