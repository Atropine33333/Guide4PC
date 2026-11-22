#include <stdio.h>

int main(void) {
    unsigned k = 0;
    if (scanf("%u", &k) != 1) return 0;

    /* TODO: 先算出 1u << k，再按题目要求输出两行。 */
    (void)k;
    printf("0 0\n");
    printf("0\n");
    return 0;
}
