#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 0 || n > 50) return 0;

    /* TODO: 输出表头。 */

    for (int i = 0; i < n; ++i) {
        char name[11];
        int score;
        if (scanf("%10s %d", name, &score) != 2) return 0;

        /* TODO: 按题面的列宽输出一行。 */
        (void)name;
        (void)score;
    }
    return 0;
}
