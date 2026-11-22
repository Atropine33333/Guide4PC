// 参考实现：c-char-signedness
#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    /* 8 位补码：最高位为 1 时，数值减去 2^8。 */
    int value = (n >= 128) ? n - 256 : n;

    printf("%d\n", value);
    return 0;
}
