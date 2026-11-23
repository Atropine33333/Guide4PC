// 参考实现：c-unsigned-compare
#include <stdio.h>

int main(void) {
    unsigned n = 0;
    if (scanf("%u", &n) != 1 || n == 0 || n > 10) return 0;

    unsigned i = n;
    while (i > 0) {
        printf("%u", i);
        --i;
        if (i > 0) {
            printf(" ");
        }
    }
    printf("\n");
    return 0;
}
