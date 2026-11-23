// 参考实现：c-bitmask-flags
#include <stdio.h>
#include <string.h>

int main(void) {
    unsigned mask = 0;
    char op[16];
    int k;

    while (scanf("%15s %d", op, &k) == 2) {
        unsigned bit = 1u << k;

        if (strcmp(op, "set") == 0) {
            mask |= bit;
        } else if (strcmp(op, "clear") == 0) {
            mask &= ~bit;
        } else if (strcmp(op, "toggle") == 0) {
            mask ^= bit;
        } else if (strcmp(op, "test") == 0) {
            printf("%u %d\n", mask, (mask & bit) ? 1 : 0);
            continue;
        }

        printf("%u\n", mask);
    }

    return 0;
}
