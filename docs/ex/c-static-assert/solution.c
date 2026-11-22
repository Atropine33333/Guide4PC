// 参考实现：c-static-assert
#include <stdio.h>

_Static_assert(sizeof(int) == 4, "int must be 4 bytes");

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    printf("n=%d int=%d check=%s\n", n, (int)sizeof(int),
           sizeof(int) == 4 ? "ok" : "bad");
    return 0;
}
