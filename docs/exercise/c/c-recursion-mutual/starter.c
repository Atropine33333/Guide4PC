#include <stdio.h>

int is_even(int n);
int is_odd(int n);

int is_even(int n) {
    /* TODO: n 为 0 时返回 1；否则返回 is_odd(n - 1)。 */
    (void)n;
    return 0;
}

int is_odd(int n) {
    /* TODO: n 为 0 时返回 0；否则返回 is_even(n - 1)。 */
    (void)n;
    return 0;
}

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    printf("%s\n", is_even(n) ? "even" : "odd");
    return 0;
}
