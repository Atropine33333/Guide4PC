#include <stdio.h>

#define SQ(v) ((v) * (v))

static int square(int v) {
    return v * v;
}

int main(void) {
    int x = 0;
    if (scanf("%d", &x) != 1) return 0;

    /* TODO: 输出 SQ(x + 1)、SQ((x) + 1)、square(x + 1)。 */
    (void)square;
    printf("0 0 0\n");
    return 0;
}
