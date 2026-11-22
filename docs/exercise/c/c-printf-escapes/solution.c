// 参考实现：c-printf-escapes
#include <stdio.h>
#include <string.h>

static void name_char(char ch) {
    switch (ch) {
    case '\n':
        fputs("\\n", stdout);
        break;
    case '\t':
        fputs("\\t", stdout);
        break;
    case '\r':
        fputs("\\r", stdout);
        break;
    case '\\':
        fputs("\\\\", stdout);
        break;
    case '"':
        fputs("\\\"", stdout);
        break;
    default:
        putchar(ch);
        break;
    }
}

int main(void) {
    char line[1100];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[--len] = '\0';

    for (size_t k = 0; k < len; ++k) {
        if (k > 0) putchar(' ');
        name_char(line[k]);
    }
    putchar('\n');
    return 0;
}
