#include <stdio.h>

#define MAX_N 200000

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 1 || n > MAX_N) return 0;

    static int a[MAX_N];
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1) return 0;
    }

    int key;
    if (scanf("%d", &key) != 1) return 0;

    /* TODO: 二分查找 key 在 a[] 里的下标（从 0 起）。
       lo 取 0，hi 取 n - 1；每轮看中点 mid = lo + (hi - lo) / 2，
       相等就停；key 比中点大，让 lo = mid + 1；
       key 比中点小，让 hi = mid - 1；范围空了就说明没有。 */

    int pos = -1;
    printf("%d\n", pos);
    return 0;
}
