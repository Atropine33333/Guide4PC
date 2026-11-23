#include <stdio.h>

int main(void) {
    char line[256];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    int x = 0;
    int y = 0;
    /* TODO: 用 sscanf 从 line 里把 x 与 y 两个整数解析出来。 */
    (void)x;
    (void)y;
    printf("%d\n", 0);
    return 0;
}
