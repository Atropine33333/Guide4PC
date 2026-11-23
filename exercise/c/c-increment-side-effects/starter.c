#include <stdio.h>

int main(void) {
    int i = 0;
    if (scanf("%d", &i) != 1) return 0;

    /* TODO: 按顺序写三条语句，把 i++、++i、i-- 各自的取值记进 a、b、c。 */
    int a = 0;
    int b = 0;
    int c = 0;

    printf("%d %d %d %d\n", a, b, c, i);
    return 0;
}
