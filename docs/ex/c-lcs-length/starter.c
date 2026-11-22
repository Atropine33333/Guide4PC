#include <stdio.h>
#include <string.h>

#define LCS_MAX 2000

static int prev_row[LCS_MAX + 1];

int main(void) {
    char a[LCS_MAX + 1];
    char b[LCS_MAX + 1];
    if (scanf("%2000s", a) != 1) return 0;
    if (scanf("%2000s", b) != 1) return 0;

    size_t n = strlen(a);
    size_t m = strlen(b);
    (void)n;

    /* TODO: 逐行推进，用一个滚动数组保存上一行的结果。 */
    for (size_t j = 0; j <= m; ++j) {
        prev_row[j] = 0;
    }

    printf("%d\n", prev_row[m]);
    return 0;
}
