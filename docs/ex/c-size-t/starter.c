#include <stdio.h>
#include <string.h>

int main(void) {
    char line[1005];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[--len] = '\0';
    }

    /* TODO: 输出行长 len 与 (int)len，中间用一个空格隔开。 */
    printf("0 0\n");
    return 0;
}
