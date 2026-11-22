#include <stdio.h>

int main(void) {
    unsigned n;
    if (scanf("%u", &n) != 1) return 0;

    for (unsigned i = n; i >= 0; --i) {
        printf("%u ", i);
    }
    printf("\n");
    return 0;
}
