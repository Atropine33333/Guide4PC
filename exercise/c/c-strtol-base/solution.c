// 参考实现：c-strtol-base
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char token[64];
    if (scanf("%63s", token) != 1) return 0;

    long value = strtol(token, NULL, 0);
    printf("%ld\n", value);
    return 0;
}
