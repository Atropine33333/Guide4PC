#include <stdio.h>

int main(void) {
    unsigned n = 0;
    if (scanf("%u", &n) != 1 || n == 0 || n > 10) return 0;

    /* TODO: 从 n 开始向下报数，报到 1 就停。
       每一步先输出当前数，再让 unsigned 的 i 减一；
       想清楚循环条件写成什么，才不会在 0 的下一格停不下来。 */
    unsigned i = n;
    (void)i;

    printf("\n");
    return 0;
}
