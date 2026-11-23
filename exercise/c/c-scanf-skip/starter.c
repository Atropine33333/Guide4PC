#include <stdio.h>

int main(void) {
    int a = 0;
    int b = 0;
    /* TODO: 用一个格式串跳过分隔符，把两段赋值里的整数一次读出来。 */
    if (scanf("%d %d", &a, &b) != 2) return 0;
    printf("%d\n", a + b);
    return 0;
}
