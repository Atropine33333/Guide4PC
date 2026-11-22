#include <stdio.h>
#include <string.h>

int main(void) {
    char line[256];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    char wanted = 0;
    if (scanf(" %c", &wanted) != 1) return 0;

    int count = 0;
    /* TODO: 用 strchr 从当前位置往后找，找到一个就把指针挪到它后面继续。 */
    const char *p = line;
    (void)p;
    (void)wanted;

    printf("%d\n", count);
    return 0;
}
