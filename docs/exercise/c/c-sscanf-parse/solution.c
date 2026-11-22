// 参考实现：c-sscanf-parse
#include <stdio.h>

int main(void) {
    char line[256];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    int x = 0;
    int y = 0;
    if (sscanf(line, " x = %d , y = %d", &x, &y) != 2) return 0;
    printf("%d\n", x + y);
    return 0;
}
