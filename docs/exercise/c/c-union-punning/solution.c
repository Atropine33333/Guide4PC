// 参考实现：c-union-punning
#include <stdio.h>
#include <stdlib.h>

union U {
    unsigned u;
    float f;
};

int main(void) {
    char plate[32];
    if (scanf("%31s", plate) != 1) return 0;

    union U plot;
    plot.u = (unsigned)strtoul(plate, NULL, 16);

    printf("%.6g\n", (double)plot.f);
    return 0;
}
