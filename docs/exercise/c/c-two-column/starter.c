#include <stdio.h>

int main(void) {
    char line[256];

    while (fgets(line, sizeof line, stdin) != NULL) {
        /* TODO：取这一行前两个整数，算出和，把三栏写出来。 */
        (void)line;
    }

    /* TODO：把有效行数与总和记下来，作废的行另记一笔，最后报合计。 */
    printf("total 0 sum 0 skipped 0\n");
    return 0;
}
