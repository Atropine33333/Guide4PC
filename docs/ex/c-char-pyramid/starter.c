#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    for (int i = 1; i <= n; ++i) {
        /* TODO: 先输出 n-i 个空格。 */
        for (int s = 0; s < n - i; ++s) {
            putchar(' ');
        }
        /* TODO: 正着写到这一行的中间字母，再倒着写回来。 */
        for (int j = 0; j < i; ++j) {
            (void)j;
        }
        putchar('\n');
    }

    return 0;
}
