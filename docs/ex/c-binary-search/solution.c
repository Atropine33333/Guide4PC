// 参考实现：c-binary-search
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

    int lo = 0;
    int hi = n - 1;
    int pos = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == key) {
            pos = mid;
            break;
        } else if (a[mid] < key) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    printf("%d\n", pos);
    return 0;
}
