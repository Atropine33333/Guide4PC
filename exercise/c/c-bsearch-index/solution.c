// 参考实现：c-bsearch-index
#include <stdio.h>
#include <stdlib.h>

#define MAX_N 1000

static int compare_int(const void *lhs, const void *rhs) {
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

    int *hit = bsearch(&key, values, (size_t)n, sizeof values[0], compare_int);
    if (hit == NULL) {
        printf("-1\n");
    } else {
        printf("%d\n", (int)(hit - values));
    }
    return 0;
}
