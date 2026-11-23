#include <stdio.h>

#define N_MAX 1000

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int values[N_MAX];
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &values[i]) != 1) return 0;
    }

    /* TODO: 先数出每个号码出现了几次，再按号码从小到大的顺序依次写出。 */
    (void)values;

    putchar('\n');
    return 0;
}
