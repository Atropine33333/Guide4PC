#include <stdio.h>

int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    /* BUG: 算出来的 result 没有交回去。 */
}

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    printf("%d\n", factorial(n));

    return 0;
}
