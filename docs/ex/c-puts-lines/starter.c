#include <stdio.h>

int main(void) {
    char line[256];
    int n = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        ++n;
        /* TODO: 去掉行尾换行，再按 "L<行号>: " 的前缀输出这一行。 */
        printf("%s", line);
    }
    return 0;
}
