// 参考实现：c-switch-fallthrough
#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int sum = 0;
    switch (n) {
    case 9:
        sum += 9;
        /* fall through */
    case 8:
        sum += 8;
        /* fall through */
    case 7:
        sum += 7;
        /* fall through */
    case 6:
        sum += 6;
        /* fall through */
    case 5:
        sum += 5;
        /* fall through */
    case 4:
        sum += 4;
        /* fall through */
    case 3:
        sum += 3;
        /* fall through */
    case 2:
        sum += 2;
        /* fall through */
    case 1:
        sum += 1;
        break;
    default:
        break;
    }

    printf("%d\n", sum);
    return 0;
}
