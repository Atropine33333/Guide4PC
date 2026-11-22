// 参考实现：c-unsigned-underflow
#include <stdio.h>

int main(void) {
    unsigned a = 0;
    unsigned b = 0;
    if (scanf("%u %u", &a, &b) != 2) return 0;

    unsigned diff = a - b;
    int signed_diff = (int)diff;

    printf("%u %d\n", diff, signed_diff);
    return 0;
}
