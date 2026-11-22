#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    long long sum = 0;
    for (int i = 0; i < n - 1; ++i) {
        int value;
        if (scanf("%d", &value) != 1) return 0;
        sum += value;
    }

    printf("%lld\n", sum);
    return 0;
}
