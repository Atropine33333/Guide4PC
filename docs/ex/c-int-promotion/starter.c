#include <stdio.h>

int main(void) {
    int a = 0;
    int b = 0;
    if (scanf("%d %d", &a, &b) != 2) return 0;

    /* TODO: 把 a、b 各自转成 char 再相加，把和存进 sum。 */
    int sum = 0;
    /* TODO: 求出 a + b 这个表达式的类型大小，存进 width。 */
    int width = 0;
    /* TODO: 求出 (unsigned char)300 的值，存进 clipped。 */
    int clipped = 0;

    printf("%d %d %d\n", sum, width, clipped);
    return 0;
}
