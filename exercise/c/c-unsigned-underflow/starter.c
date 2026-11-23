#include <stdio.h>

int main(void) {
    unsigned a = 0;
    unsigned b = 0;
    if (scanf("%u %u", &a, &b) != 2) return 0;

    /* TODO: 求出 a - b 的结果，按 unsigned 存进 diff。 */
    unsigned diff = 0;
    /* TODO: 把同一个结果转成 int 存进 signed_diff。 */
    int signed_diff = 0;

    printf("%u %d\n", diff, signed_diff);
    return 0;
}
