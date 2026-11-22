// 参考实现：c-comma-operator
#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    long long sum = 0;
    int count = 0;

    for (int k = 1; k <= n; sum += k, ++count, ++k) {
    }

    printf("%lld %d\n", sum, count);
    return 0;
}
