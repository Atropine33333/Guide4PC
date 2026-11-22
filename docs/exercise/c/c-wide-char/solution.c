// 参考实现：c-wide-char
#include <stdio.h>
#include <wchar.h>

int main(void) {
    char line[1005];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    size_t len = 0;
    while (line[len] != '\0' && line[len] != '\n' && line[len] != '\r') {
        ++len;
    }

    for (size_t i = 0; i < len; ++i) {
        wchar_t wc = (wchar_t)(unsigned char)line[i];
        if (i > 0) {
            putchar(' ');
        }
        printf("%d", (int)wc);
    }
    putchar('\n');
    return 0;
}
