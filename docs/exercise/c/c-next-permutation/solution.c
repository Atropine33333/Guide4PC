// 参考实现：c-next-permutation
#include <stdio.h>

#define PERM_MAX 1000

static int a[PERM_MAX];

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1) return 0;
    }

    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) {
        --i;
    }

    if (i < 0) {
        printf("none\n");
        return 0;
    }

    int j = n - 1;
    while (a[j] <= a[i]) {
        --j;
    }

    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;

    for (int left = i + 1, right = n - 1; left < right; ++left, --right) {
        tmp = a[left];
        a[left] = a[right];
        a[right] = tmp;
    }

    for (int k = 0; k < n; ++k) {
        printf("%d%c", a[k], k + 1 == n ? '\n' : ' ');
    }
    return 0;
}
