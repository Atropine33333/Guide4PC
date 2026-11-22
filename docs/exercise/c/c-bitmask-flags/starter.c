#include <stdio.h>
#include <string.h>

int main(void) {
    unsigned mask = 0;
    char op[16];
    int k;

    while (scanf("%15s %d", op, &k) == 2) {
        /* TODO: 按 op 修改 mask，并按题目要求输出一行。 */
        (void)op;
        (void)k;
        printf("%u\n", mask);
    }

    return 0;
}
