// 参考实现：c-find-bug-swap
#include <stdio.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int main(void) {
    int x = 0;
    int y = 0;
    if (scanf("%d %d", &x, &y) != 2) return 0;

    swap(&x, &y);
    printf("%d %d\n", x, y);

    return 0;
}
