// 参考实现：c-implicit-convert
#include <stdio.h>

int main(void) {
    int a = 0;
    unsigned b = 0;
    if (scanf("%d %u", &a, &b) != 2) return 0;

    /* 有符号转无符号按模 2^32 回绕，负数因此落到很大的值上。 */
    unsigned ua = (unsigned)a;

    /* 混着比较时，int 那一侧先转成 unsigned，再按无符号比较。 */
    int less = (ua < b) ? 1 : 0;

    printf("%d %u\n", less, ua);
    return 0;
}
