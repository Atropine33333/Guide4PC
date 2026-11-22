// 参考实现：c-long-long
#include <stdio.h>

int main(void) {
    int a = 0;
    int b = 0;
    if (scanf("%d %d", &a, &b) != 2) return 0;

    long long product = (long long)a * b;
    printf("%lld\n", product);
    return 0;
}
