// 参考实现：c-strchr-count
#include <stdio.h>
#include <string.h>

int main(void) {
    char line[256];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    char wanted = 0;
    if (scanf(" %c", &wanted) != 1) return 0;

    int count = 0;
    const char *p = line;
    while ((p = strchr(p, wanted)) != NULL) {
        ++count;
        ++p;
    }

    printf("%d\n", count);
    return 0;
}
