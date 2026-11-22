// 参考实现：c-char-pyramid
#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    for (int i = 1; i <= n; ++i) {
        for (int s = 0; s < n - i; ++s) {
            putchar(' ');
        }
        for (int j = 0; j < i; ++j) {
            putchar('A' + j);
        }
        for (int j = i - 2; j >= 0; --j) {
            putchar('A' + j);
        }
        putchar('\n');
    }

    return 0;
}
