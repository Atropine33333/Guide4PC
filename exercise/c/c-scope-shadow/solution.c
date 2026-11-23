// 参考实现：c-scope-shadow
#include <stdio.h>

int main(void) {
    int x = 0;
    if (scanf("%d", &x) != 1) return 0;

    int inner = 0;
    {
        int outer = x;        /* 先把外层值存下来 */
        int x = outer + 1;    /* 内层 x 遮蔽了外层 x */
        inner = x;
    }                         /* 块结束，内层 x 的生命期到头 */

    printf("%d %d\n", inner, x);
    return 0;
}
