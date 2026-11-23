#include <stdio.h>

int main(void) {
    char word[16];
    if (scanf("%15s", word) != 1) return 0;

    for (int i = 0; word[i] != '\0'; ++i) {
        /* TODO: 按字母查出枚举量 A、B、C、D 的整数值。 */
        int value = 0;
        if (i > 0) putchar(' ');
        printf("%d", value);
    }
    putchar('\n');
    return 0;
}
