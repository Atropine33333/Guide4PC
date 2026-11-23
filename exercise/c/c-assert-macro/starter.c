#include <stdio.h>

/* TODO: 把 CHECK 改成“成立喊 OK，不成立喊 FAIL:行号”的宏。
 * 行号取宏调用处的 __LINE__，条件只求值一次。 */
#define CHECK(cond) ((void)(cond))

int main(void) {
    int a = 0;
    int b = 0;
    if (scanf("%d %d", &a, &b) != 2) return 0;

#line 40 "checks"
    CHECK(a < b);
    CHECK(a == b);
    return 0;
}
