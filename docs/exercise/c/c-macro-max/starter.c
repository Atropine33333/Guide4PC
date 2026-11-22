#include <stdio.h>

/* TODO: 把 MAX 写成一枚函数式宏，展开后每个参数各自带括号。 */
#define MAX(a, b) (a)

int main(void) {
    int a = 0;
    int b = 0;
    int c = 0;
    if (scanf("%d %d %d", &a, &b, &c) != 3) return 0;

    int best = MAX(a, b);

    /* TODO: 再与 c 比一次，把两个结果一起输出。 */
    printf("%d %d\n", best, best);
    return 0;
}
