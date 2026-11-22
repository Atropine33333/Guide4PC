// 参考实现：c-macro-side-effect
#include <stdio.h>

#define SQ(v) ((v) * (v))

static int square(int v) {
    return v * v;
}

int main(void) {
    int x = 0;
    if (scanf("%d", &x) != 1) return 0;

    printf("%d %d %d\n", SQ(x + 1), SQ((x) + 1), square(x + 1));
    return 0;
}
