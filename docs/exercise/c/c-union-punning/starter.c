#include <stdio.h>

union U {
    unsigned u;
    float f;
};

int main(void) {
    char plate[32];
    if (scanf("%31s", plate) != 1) return 0;

    union U plot;
    plot.u = 0;
    /* TODO: 把 plate 里的十六进制数读出来，原样写进 plot.u。 */
    (void)plate;

    /* TODO: 从 plot.f 读出同一块地的小数解释，按 %.6g 打印。 */
    printf("0\n");
    return 0;
}
