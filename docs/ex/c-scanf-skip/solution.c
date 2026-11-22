// 参考实现：c-scanf-skip
#include <stdio.h>

int main(void) {
    int a = 0;
    int b = 0;
    if (scanf(" a = %d%*[ ,] b = %d", &a, &b) != 2) return 0;
    printf("%d\n", a + b);
    return 0;
}
