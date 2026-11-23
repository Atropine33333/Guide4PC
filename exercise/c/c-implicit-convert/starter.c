#include <stdio.h>

int main(void) {
    int a = 0;
    unsigned b = 0;
    if (scanf("%d %u", &a, &b) != 2) return 0;

    /* TODO: 先求 a < b 的真假（1 或 0），再求 (unsigned)a。
       想清楚 a 为负数时，混着比较会发生什么。 */
    printf("0 0\n");
    return 0;
}
