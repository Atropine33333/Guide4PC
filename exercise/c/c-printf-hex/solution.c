// 参考实现：c-printf-hex
#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    unsigned int v = (unsigned int)n;
    printf("%x\n", v);
    printf("%o\n", v);

    if (v == 0u) {
        printf("0\n");
        return 0;
    }

    char bits[64];
    int len = 0;
    while (v > 0u) {
        bits[len++] = (char)('0' + (int)(v & 1u));
        v >>= 1;
    }
    for (int i = len - 1; i >= 0; --i) {
        putchar(bits[i]);
    }
    putchar('\n');
    return 0;
}
