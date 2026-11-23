#include <stdio.h>

#define PERM_MAX 1000

static int a[PERM_MAX];

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1) return 0;
    }

    /* TODO: 从右往左找出第一处上升的位置 i（a[i] < a[i + 1]）。
       找到后，在 i 右侧找出比 a[i] 大的最小数，交换并反转后缀。
       若找不到上升处，说明这已经是最后一个排列。 */

    printf("none\n");
    return 0;
}
