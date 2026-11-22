#include <stdio.h>
#include <stdlib.h>

#define MAX_N 1000

int compare_int(const void *lhs, const void *rhs) {
    int a = *(const int *)lhs;
    int b = *(const int *)rhs;
    return (a > b) - (a < b);
}

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int values[MAX_N];
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &values[i]) != 1) return 0;
    }

    int key = 0;
    if (scanf("%d", &key) != 1) return 0;

    /* TODO: 用 bsearch 在 values 里找 key，输出下标或 -1。 */
    (void)key;
    printf("-1\n");
    return 0;
}
