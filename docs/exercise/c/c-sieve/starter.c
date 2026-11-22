#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 0 || n > 1000000) return 0;

    /* TODO: 用筛法数出 2 到 n 之间的素数个数。
       可以 calloc 一个 n + 1 个字节的标记数组，
       从 2 扫到 n：遇到没标过的数就计一个，
       再把它的倍数（从它的平方起）都标上。 */

    int count = 0;
    printf("%d\n", count);
    return 0;
}
