// 参考实现：c-assert-macro
#include <stdio.h>

#define CHECK(cond) do { \
    if (cond) { \
        puts("OK"); \
    } else { \
        printf("FAIL:%d\n", __LINE__); \
    } \
} while (0)

int main(void) {
    int a = 0;
    int b = 0;
    if (scanf("%d %d", &a, &b) != 2) return 0;

#line 40 "checks"
    CHECK(a < b);
    CHECK(a == b);
    return 0;
}
