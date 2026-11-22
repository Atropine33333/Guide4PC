// 参考实现：c-lcs-length
#include <stdio.h>
#include <string.h>

#define LCS_MAX 2000

static int prev_row[LCS_MAX + 1];
static int cur_row[LCS_MAX + 1];

int main(void) {
    char a[LCS_MAX + 1];
    char b[LCS_MAX + 1];
    if (scanf("%2000s", a) != 1) return 0;
    if (scanf("%2000s", b) != 1) return 0;

    size_t n = strlen(a);
    size_t m = strlen(b);

    for (size_t j = 0; j <= m; ++j) {
        prev_row[j] = 0;
    }

    for (size_t i = 1; i <= n; ++i) {
        cur_row[0] = 0;
        for (size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                cur_row[j] = prev_row[j - 1] + 1;
            } else if (prev_row[j] >= cur_row[j - 1]) {
                cur_row[j] = prev_row[j];
            } else {
                cur_row[j] = cur_row[j - 1];
            }
        }
        for (size_t j = 0; j <= m; ++j) {
            prev_row[j] = cur_row[j];
        }
    }

    printf("%d\n", prev_row[m]);
    return 0;
}
