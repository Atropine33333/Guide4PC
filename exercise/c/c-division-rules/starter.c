#include <stdio.h>

int main(void) {
    int a = 0;
    int b = 0;
    if (scanf("%d %d", &a, &b) != 2 || b == 0) return 0;

    int q = 0;
    int r = 0;

    /* TODO: 按 C 的截断除法算出商 q 与余数 r。 */

    printf("%d %d\n", q, r);
    return 0;
}
