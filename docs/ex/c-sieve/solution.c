// 参考实现：c-sieve
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 0 || n > 1000000) return 0;
    if (n < 2) {
        printf("0\n");
        return 0;
    }

    unsigned char *composite = calloc((size_t)n + 1, 1);
    if (composite == NULL) return 1;

    int count = 0;
    for (int i = 2; i <= n; ++i) {
        if (!composite[i]) {
            ++count;
            if ((long long)i * i <= n) {
                for (int j = i * i; j <= n; j += i) {
                    composite[j] = 1;
                }
            }
        }
    }

    free(composite);
    printf("%d\n", count);
    return 0;
}
