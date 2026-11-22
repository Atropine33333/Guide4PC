// 参考实现：c-increment-side-effects
#include <stdio.h>

int main(void) {
    int i = 0;
    if (scanf("%d", &i) != 1) return 0;

    int a = i++;
    int b = ++i;
    int c = i--;

    printf("%d %d %d %d\n", a, b, c, i);
    return 0;
}
