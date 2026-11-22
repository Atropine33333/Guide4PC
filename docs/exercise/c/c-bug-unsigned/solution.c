// 参考实现：c-bug-unsigned
#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    for (int i = n; i >= 1; --i) {
        printf("%d", i);
        if (i > 1) putchar(' ');
    }
    putchar('\n');
    return 0;
}
