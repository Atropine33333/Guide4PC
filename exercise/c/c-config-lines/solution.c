// 参考实现：c-config-lines
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 205

static char *trim(char *text) {
    while (*text != '\0' && isspace((unsigned char)*text)) {
        ++text;
    }
    size_t len = strlen(text);
    while (len > 0 && isspace((unsigned char)text[len - 1])) {
        text[--len] = '\0';
    }
    return text;
}

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* 吃掉第一行剩下的换行符。 */
    }

    char line[MAX_LINE];
    for (int i = 0; i < n; ++i) {
        if (fgets(line, sizeof line, stdin) == NULL) break;

        char *text = trim(line);
        if (*text == '\0' || *text == '#') continue;

        char *eq = strchr(text, '=');
        if (eq == NULL) continue;
        *eq = '\0';

        char *key = trim(text);
        char *value = trim(eq + 1);
        printf("%s: %s\n", key, value);
    }

    return 0;
}
