// 参考实现：c-recursion-mutual
#include <stdio.h>

int is_even(int n);
int is_odd(int n);

int is_even(int n) {
    if (n == 0) return 1;
    return is_odd(n - 1);
}

int is_odd(int n) {
    if (n == 0) return 0;
    return is_even(n - 1);
}

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    printf("%s\n", is_even(n) ? "even" : "odd");
    return 0;
}
