#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int total = 0;
    for (int i = 0; i < n; ++i) {
        int value;
        if (scanf("%d", &value) != 1) return 0;
        total += value;
    }

    printf("%d\n", total);
    return 0;
}
