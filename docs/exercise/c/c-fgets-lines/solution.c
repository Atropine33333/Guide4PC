// 参考实现：c-fgets-lines
#include <stdio.h>
#include <string.h>

int main(void) {
    char line[256];
    int n = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }
        ++n;
        if (len == 0) {
            printf("[%d]\n", n);
        } else {
            printf("[%d] %s\n", n, line);
        }
    }
    return 0;
}
