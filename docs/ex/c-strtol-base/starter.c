#include <stdio.h>

int main(void) {
    char token[64];
    if (scanf("%63s", token) != 1) return 0;

    long value = 0;
    /* TODO: 用 strtol 按键盘上的前缀选进制，把 token 转成数值。 */
    (void)token;
    printf("%ld\n", value);
    return 0;
}
