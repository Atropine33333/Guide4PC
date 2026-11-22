#include <stdio.h>

int main(void) {
    int a = 0;
    int b = 0;
    if (scanf("%d %d", &a, &b) != 2) return 0;

    /* TODO: 分别求下面两个表达式的真假值，用 0/1 记下结果。
       注意 b 为 0 时不能真的做除法。 */
    (void)a;
    (void)b;
    int first = 0;
    int second = 0;

    printf("%d %d\n", first, second);
    return 0;
}
