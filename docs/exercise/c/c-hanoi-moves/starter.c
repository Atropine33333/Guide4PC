#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 1 || n > 30) return 0;

    /* TODO: 记搬 n 个盘子的最少步数为 T(n)。
       把最大的盘子挪到目标柱之前，得先把上面 n - 1 个挪开，
       搬完最大的再把这 n - 1 个挪回来，所以
       T(n) = 2 * T(n - 1) + 1，T(1) = 1。
       也可以直接输出 2 的 n 次方减一。 */

    long long moves = 0;
    printf("%lld\n", moves);
    return 0;
}
