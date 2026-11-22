// 参考实现：c-u8-wrap
#include <stdio.h>
#include <stdint.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    uint8_t acc = 0;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        if (scanf("%d", &value) != 1) break;

        acc = (uint8_t)(acc + (uint8_t)value);
        printf("%u\n", (unsigned)acc);
    }

    return 0;
}
