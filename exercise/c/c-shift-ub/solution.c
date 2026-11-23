// 参考实现：c-shift-ub
#include <stdio.h>

int main(void) {
    unsigned k = 0;
    if (scanf("%u", &k) != 1) return 0;

    unsigned value = 1u << k;
    printf("%u %u\n", value, value - 1u);
    printf("%u\n", 1u << 31);

    return 0;
}
