#include <stdio.h>

int main(void) {
    int x = 0;
    if (scanf("%d", &x) != 1) return 0;

    /* TODO: 进入一层花括号，先在块里保存外层 x，再声明一个同名的 x，
       让它等于外层值加 1；块外再打印外层 x。目前两处都还是占位输出。 */
    int inner = x;

    printf("%d %d\n", inner, x);
    return 0;
}
