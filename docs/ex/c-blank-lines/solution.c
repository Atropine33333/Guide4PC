// 参考实现：c-blank-lines
#include <stdio.h>
#include <string.h>

static int is_blank(const char *line) {
    for (const char *p = line; *p != '\0'; ++p) {
        if (*p != ' ' && *p != '\t') return 0;
    }
    return 1;
}

int main(void) {
    char line[1100];
    int printed = 0;
    int pending_blank = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        if (is_blank(line)) {
            if (printed) pending_blank = 1;
            continue;
        }

        if (pending_blank) {
            putchar('\n');
            pending_blank = 0;
        }
        fputs(line, stdout);
        putchar('\n');
        printed = 1;
    }
    return 0;
}
