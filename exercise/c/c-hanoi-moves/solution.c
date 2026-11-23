// 参考实现：c-hanoi-moves
#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 1 || n > 30) return 0;

    long long moves = (1LL << n) - 1;
    printf("%lld\n", moves);
    return 0;
}
