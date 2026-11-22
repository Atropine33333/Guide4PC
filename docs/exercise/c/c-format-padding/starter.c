#include <stdio.h>

int main(void) {
    int width = 0;
    int n = 0;
    if (scanf("%d %d", &width, &n) != 2) return 0;

    /* TODO：让 width 也参与排版，宽度为正靠右、为负靠左。 */
    printf("[%d]\n", n);

    (void)width;
    return 0;
}
