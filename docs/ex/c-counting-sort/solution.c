// 参考实现：c-counting-sort
#include <stdio.h>

#define VALUE_MAX 100

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int count[VALUE_MAX + 1] = {0};
    for (int i = 0; i < n; ++i) {
        int value = 0;
        if (scanf("%d", &value) != 1) return 0;
        if (value >= 0 && value <= VALUE_MAX) {
            ++count[value];
        }
    }

    int first = 1;
    for (int value = 0; value <= VALUE_MAX; ++value) {
        for (int k = 0; k < count[value]; ++k) {
            if (!first) putchar(' ');
            printf("%d", value);
            first = 0;
        }
    }
    putchar('\n');
    return 0;
}
