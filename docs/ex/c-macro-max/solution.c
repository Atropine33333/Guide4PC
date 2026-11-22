// 参考实现：c-macro-max
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void) {
    int a = 0;
    int b = 0;
    int c = 0;
    if (scanf("%d %d %d", &a, &b, &c) != 3) return 0;

    int best = MAX(a, b);
    printf("%d %d\n", best, MAX(best, c));
    return 0;
}
