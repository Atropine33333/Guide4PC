#include <stdio.h>

int main(void) {
    char line[256];
    int n = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        ++n;
        /* TODO: 去掉行尾的换行，再输出 "[编号] 原行"。 */
        printf("%s", line);
    }
    return 0;
}
