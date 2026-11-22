// 参考实现：c-size-t
#include <stdio.h>
#include <string.h>

int main(void) {
    char line[1005];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    size_t len = strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
        line[--len] = '\0';
    }

    size_t n = strlen(line);
    printf("%zu %d\n", n, (int)n);
    return 0;
}
