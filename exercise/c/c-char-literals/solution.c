// 参考实现：c-char-literals
#include <stdio.h>

int main(void) {
    int k = 0;
    if (scanf("%d", &k) != 1) return 0;

    printf("%d %d %d %d %d\n", 'A' + k, '\n', '\x41', '\\', '0');
    return 0;
}
