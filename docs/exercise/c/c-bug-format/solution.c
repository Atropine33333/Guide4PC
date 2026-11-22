// 参考实现：c-bug-format
#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    for (int i = 1; i <= n; ++i) {
        printf("%4d%6d\n", i, i * i);
    }

    return 0;
}
