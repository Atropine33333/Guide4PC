#include <stdio.h>
#include <stdlib.h>

#define MAX_TERMS 1000

int main(void) {
    int m = 0;
    int n = 0;
    int k = 0;
    if (scanf("%d %d %d", &m, &n, &k) != 3) return 0;
    if (k < 0) k = 0;
    if (k > MAX_TERMS) k = MAX_TERMS;

    int rows[MAX_TERMS];
    int cols[MAX_TERMS];
    int vals[MAX_TERMS];
    for (int i = 0; i < k; ++i) {
        if (scanf("%d %d %d", &rows[i], &cols[i], &vals[i]) != 3) return 1;
    }

    /* TODO: 转置后按新行、新列升序输出三元组。 */
    printf("%d %d %d\n", n, m, k);

    return 0;
}
